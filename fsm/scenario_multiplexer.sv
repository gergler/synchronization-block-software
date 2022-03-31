//import bus_package::*;
`include "bus_package.sv"

module scenario_multiplexer (
	input clock,
	input [7:0] scen_sel,
    input scen_start,
    input scen_reset,
        
    input_ports_bus      input_ports,
    parameters_ports_bus parameters_ports,
    output_ports_bus     output_ports
);

//output_ports_bus output_ports_bus_n[4];
output_ports_bus output_ports_bus_0();
output_ports_bus output_ports_bus_1();
output_ports_bus output_ports_bus_2();
output_ports_bus output_ports_bus_3();

enum logic [7:0] {EXPERIMENT, EXPERIMENT_PHASE, CALIBRATION, CALIBRATION_PHASE} scenario;

/*
assign output_ports_bus_0 = '{0, 0, 0, 0};
assign output_ports_bus_1 = '1;
assign output_ports_bus_2 = '0;
assign output_ports_bus_3 = '1;

assign output_ports_bus_n[0] = '0;
assign output_ports_bus_n[1] = '1;
assign output_ports_bus_n[2] = '0;
assign output_ports_bus_n[3] = '1;
*/

assign output_ports_bus_0.detonation_signal = 'Z;
assign output_ports_bus_0.output_trigger    = 'Z;
assign output_ports_bus_0.scenario_state    = '0; 
assign output_ports_bus_0.counter_out       = '0;

assign output_ports_bus_1.detonation_signal = '1;
assign output_ports_bus_1.output_trigger    = '1;
assign output_ports_bus_1.scenario_state    = '1; 
assign output_ports_bus_1.counter_out       = '1;

assign output_ports_bus_2.detonation_signal = '0;
assign output_ports_bus_2.output_trigger    = '0;
assign output_ports_bus_2.scenario_state    = '0; 
assign output_ports_bus_2.counter_out       = '0;

assign output_ports_bus_3.detonation_signal = 'Z;
assign output_ports_bus_3.output_trigger    = 'Z;
assign output_ports_bus_3.scenario_state    = 'Z; 
assign output_ports_bus_3.counter_out       = 'Z;

always_comb begin
        // output_ports_bus <= output_ports_bus_n[scen_sel];
		unique case (scen_sel)
        0 : begin
            output_ports.detonation_signal <= output_ports_bus_0.detonation_signal;
            output_ports.output_trigger    <= output_ports_bus_0.output_trigger;
            output_ports.scenario_state    <= output_ports_bus_0.scenario_state; 
            output_ports.counter_out       <= output_ports_bus_0.counter_out;
        end
        1 : begin
            output_ports.detonation_signal <= output_ports_bus_1.detonation_signal;
            output_ports.output_trigger    <= output_ports_bus_1.output_trigger;
            output_ports.scenario_state    <= output_ports_bus_1.scenario_state; 
            output_ports.counter_out       <= output_ports_bus_1.counter_out;
        end
        2 : begin
            output_ports.detonation_signal <= output_ports_bus_2.detonation_signal;
            output_ports.output_trigger    <= output_ports_bus_2.output_trigger;
            output_ports.scenario_state    <= output_ports_bus_2.scenario_state; 
            output_ports.counter_out       <= output_ports_bus_2.counter_out;
        end
        3 : begin
            output_ports.detonation_signal <= output_ports_bus_3.detonation_signal;
            output_ports.output_trigger    <= output_ports_bus_3.output_trigger;
            output_ports.scenario_state    <= output_ports_bus_3.scenario_state; 
            output_ports.counter_out       <= output_ports_bus_3.counter_out;
        end
        default : begin
            output_ports.detonation_signal <= 'Z;
            output_ports.output_trigger    <= 'Z;
            output_ports.scenario_state    <= 'Z;
            output_ports.counter_out       <= 'Z;
        end

        endcase
        
        /*
        case (scen_sel)
			EXPERIMENT: begin
				output_ports <= '{0, 0, 0, 0};
			end
			
			EXPERIMENT_PHASE: begin
				output_ports <= '{0, 0, 0, 0};
			end
			
			CALIBRATION: begin
				output_ports <= '{'z, 0, 0, 0};
			end
			
			CALIBRATION_PHASE: begin
				output_ports <= '{'z, 0, 0, 0};
			end
			
			default: begin
				output_ports <= '{'z, 'z, 'z, 'z};
			end
		
		endcase  */
end

endmodule
