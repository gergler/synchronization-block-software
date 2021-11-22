`timescale 1ps/1ps  

module freDetect_tb;  
  parameter CLK = 1; // опорный сигнал  
  parameter TEST_DELAY = 20; // тестируемый сигнал  
  reg clk;  
  reg test;  
  reg rst;  
    
  wire d;  
    
  initial  
    begin  
      rst =1'b0;  
      #1 rst = 1'b1;  
    end  
      
  initial  
    begin  
      test = 1'b0;  
      forever #TEST_DELAY test = ~test;  
    end  
      
  initial  
    begin  
      clk = 1'b0;  
      forever #CLK clk = ~clk;  
    end  
      
    freDetect freDetect1(.clk(clk), .rst(rst), .test(test), .d(d));  
      
     
endmodule  
