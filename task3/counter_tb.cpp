#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    //init top verilog instance
    Vcounter* top = new Vcounter;
    //init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("countertask3.vcd");

    //init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");

    //initialise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;
    top->v = vbdValue();

    vbdSetMode(1);

    //run simulation for many clock cycles
    for(i=0; i<50; i++){
        
        //dump variables into VCD file and toggle clock
        for(clk = 0; clk<2; clk++){
            tfp->dump (2*i+clk); //unit is ps
            top->clk = !top->clk;
            top->eval ();

        }

        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);      
        //vbdPlot(int(top->count), 0, 255);
  
        
        vbdCycle(i+1);
        //end of vbuddy output section

    //change input stimuli
        top->rst = (i < 2) | (i == 15);
        top->ld = vbdFlag();
        top->v = vbdValue();

    // either simulation finished, or 'q' is pressed
    if ((Verilated::gotFinish()) || (vbdGetkey()=='q')) 
      exit(0);                // ... exit if finish OR 'q' pressed    
    }

    vbdClose();
    tfp->close();
    exit(0);
}