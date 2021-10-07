module tb_main();

reg clk;
main dut(clk);

initial begin
	clk = 0;
	forever #1 clk = ~clk;
end

endmodule
