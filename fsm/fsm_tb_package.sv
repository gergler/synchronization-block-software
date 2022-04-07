module clock_generate #(parameter PERIOD=20) (output logic clock);
	initial forever begin
		clock = 0;
		#(PERIOD/2);
		clock = 1;
		#(PERIOD/2);
	end
endmodule


module reset_generate(output logic reset);
	initial begin
		reset = 1;
		#100ns;
		reset = 0;
	end
endmodule


module start_generate(output logic start);
	initial begin
		#5ms;
		start = 1;
		#100us;
		start = 0;
	end
endmodule


module fg_opto_generate(output logic fg_opto, 
						input logic fg_period, fg_opened);
	initial begin
		$urandom(1);
		#($urandom_range(FG_PERIOD/2, FG_PERIOD/8));
		repeat(10) begin 
			fg_opto = 1;
			#fg_opened;
			fg_opto = 0;
			#(fg_period - fg_opened);
		end
	end
endmodule


module fg_open_generate(output logic fg_open, 
						input logic fg_opto, fg_opened);
	always @(posedge fg_opto) begin
		#9ms;
		fg_open = 1;
		#fg_opened;
		fg_open = 0;
	end
endmodule


module wire_sensor_generate(input logic detonator_triggered, reset,
							output logic wire_sensor
);
	always @(posedge detonator_triggered) begin
		if (reset == '0) begin
			#5us;
			wire_sensor = 1;
			for (int i = 0; i < 10; i++) begin
				#( 10*(1+($urandom() % 10)) );
				wire_sensor = ~wire_sensor;
			end
			#100;
			wire_sensor = 1;
			#1ms;
			wire_sensor = 0;
		end
	end
endmodule


module detector_ready_generate(input logic output_trigger, reset, detector_prolong, 
							   output logic detector_ready
);
	always @(posedge output_trigger) begin
		if (reset == '0) begin
			#200ns;
			detector_ready = 0;
			#detector_prolong;
			detector_ready = 1;
		end;
	end
endmodule
