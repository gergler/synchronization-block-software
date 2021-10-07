module main(clk);

input clk;

reg[79:0] counter;
reg [7:0] led_msk = 8'b1;
assign LED = led_msk;

always @(posedge clk) begin
	   if (counter  < 50)
			counter <= counter + 1;
		else begin
			led_msk[7:0] <= { (led_msk[7:0] << 1'b1) | led_msk[7] };
			counter <= 80'd0;
		end
	end 
endmodule
