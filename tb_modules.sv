`timescale 1ns/10ps

module clock_module #(parameter PERIOD=20) (output logic clock);
	initial forever begin
		clock = 0;
		#(PERIOD/2.0);
		clock = 1;
        #(PERIOD/2.0);
	end
endmodule


module reset_module(output logic reset);
	initial begin
		reset = 1;
		#100ns;
		reset = 0;
	end
endmodule


module start_module(output logic start);  
	initial begin
		start = 0;
		#5ms;
		start = 1;
		#100us;
		start = 0;
	end
endmodule


module fg_opto_module(output logic fg_opto);
    parameter FG_PERIOD = 10_000_000;
    parameter FG_OPENED =    400_000/5;
	initial begin
        fg_opto = 0;
		$urandom(1);
		#($urandom_range(FG_PERIOD/2, FG_PERIOD/8));
		forever begin 
			fg_opto = 1;
			#FG_OPENED;
			fg_opto = 0;
			#(FG_PERIOD - FG_OPENED);
		end
	end
endmodule


module fg_open_module(input logic reset, fg_opto,
					  output logic fg_open
);
    parameter FG_OPENED = 400_000/5;
    always @(posedge fg_opto or posedge reset) begin
        if (reset)
            fg_open = 0;
        else begin
            #5ms;
            fg_open = 1;
            #FG_OPENED;
            fg_open = 0;
        end
    end
endmodule


module wire_sensor_module(input logic reset, detonator_triggered,
                          output logic wire_sensor
);
	always @(posedge detonator_triggered or posedge reset) begin
		if (reset)
            wire_sensor = 0;
        else begin
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


module detector_ready_module(input logic reset, output_trigger, 
							 output logic detector_ready
);
    parameter DETECTOR_PROLONG = 6400us;
	always @(posedge output_trigger or posedge reset) begin
		if (reset)
            detector_ready = 1;
        else begin
			#200ns;
			detector_ready = 0;
			#DETECTOR_PROLONG;
			detector_ready = 1;
		end;
	end
endmodule
