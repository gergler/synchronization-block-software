`ifndef GUARD_FSM_TB_PACKAGE
`define GUARD_FSM_TB_PACKAGE

	package fsm_tb_package;
		
		task automatic reset_generate(ref reset, input time delay);
			reset = 1;
			#delay;
			reset = 0;
		endtask
		
		task automatic start_generate(ref start, input time delay_1, delay_2);
			#delay_1;
			start = 1;
			#delay_2;
			start = 0;
		endtask
		
		task automatic fg_opto_generate(ref fg_opto, input time fg_period, fg_opened);
			fg_opto = 1;
            #fg_opened;
            fg_opto = 0;
            #(fg_period - fg_opened);
		endtask
		
		task automatic fg_open_generate(ref fg_open, input time fg_opened);
			#2ms;
			fg_open = 1;
			#fg_opened;
			fg_open = 0;
		endtask
		
		task automatic wire_sensor_generate(ref wire_sensor);
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
		endtask
		
		task automatic detector_ready_generate(ref detector_ready, input time detector_prolong);
            detector_ready = 0;
            #detector_prolong;
            detector_ready = 1;
		endtask
		
	endpackage
	
`endif