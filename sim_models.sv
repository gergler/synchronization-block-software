`timescale 1ns/10ps

import types_pkg::*;


module clock_model #(parameter PERIOD=20) (output logic clock);

	initial forever begin
		clock = 0;
		#(PERIOD/2);
		clock = 1;
		#(PERIOD/2);
	end

endmodule


module data_generator(output input_signals_t data);

	initial forever begin
        for (int i=0; i<8; ++i) begin
            data = i;
            #100ns;
        end
	end

endmodule
