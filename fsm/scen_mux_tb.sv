`timescale 1ns/10ps
`include "bus_package.sv"

module scen_mux_tb;
	logic clock = 0;
    logic scen_reset = 0;
    logic scen_start = 0;
	logic [7:0] scen_sel = 0;
    
    logic output_trigger;
	
	localparam CLOCK = 2.5;
	
	always clock = #CLOCK ~clock;
	
	initial begin
		/*for (control_reg = 0; control_reg < 9; control_reg = control_reg + 1) begin
			#10us;
			if (control_reg == 4)
				control_reg = 0;
		end*/
        
        scen_sel = 0;
        #10us;
        scen_sel = 1;
        #10us;
        scen_sel = 2;
        #10us;
        scen_sel = 3;
        #10us;
	end
	
    input_ports_bus      input_ports();
    parameters_ports_bus parameters_ports();
    output_ports_bus     output_ports();
    
    assign output_trigger = output_ports.output_trigger;
    
	scenario_multiplexer scenario_mux(.*);
    
endmodule
