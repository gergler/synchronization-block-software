module top(


	//////////// CLOCK //////////
	input 		          		CLOCK_50,
	input 		          		FPGA_CLK2_50,
	input 		          		FPGA_CLK3_50,

`ifdef enable_ADC
	//////////// ADC //////////
	/* 3.3-V LVTTL */
	output		          		ADC_CONVST,
	output		          		ADC_SCLK,
	output		          		ADC_SDI,
	input 		          		ADC_SDO,
`endif
	
`ifdef enable_ARDUINO
	//////////// ARDUINO ////////////
	/* 3.3-V LVTTL */
	inout					[15:0]	ARDUINO_IO,
	inout								ARDUINO_RESET_N,
`endif
	
`ifdef enable_GPIO0
	//////////// GPIO 0 ////////////
	/* 3.3-V LVTTL */
	inout				[35:0]		GPIO_0,
`endif

`ifdef enable_GPIO1	
	//////////// GPIO 1 ////////////
	/* 3.3-V LVTTL */
	inout				[35:0]		GPIO_1,
`endif

	input			[35:0]		GPIO_1,


`ifdef enable_HPS
	//////////// HPS //////////
	/* 3.3-V LVTTL */
	inout 		          		HPS_CONV_USB_N,
	
	/* SSTL-15 Class I */
	output		    [14:0]		HPS_DDR3_ADDR,
	output		     [2:0]		HPS_DDR3_BA,
	output		          		HPS_DDR3_CAS_N,
	output		          		HPS_DDR3_CKE,
	output		          		HPS_DDR3_CS_N,
	output		     [3:0]		HPS_DDR3_DM,
	inout 		    [31:0]		HPS_DDR3_DQ,
	output		          		HPS_DDR3_ODT,
	output		          		HPS_DDR3_RAS_N,
	output		          		HPS_DDR3_RESET_N,
	input 		          		HPS_DDR3_RZQ,
	output		          		HPS_DDR3_WE_N,
	/* DIFFERENTIAL 1.5-V SSTL CLASS I */
	output		          		HPS_DDR3_CK_N,
	output		          		HPS_DDR3_CK_P,
	inout 		     [3:0]		HPS_DDR3_DQS_N,
	inout 		     [3:0]		HPS_DDR3_DQS_P,
	
	/* 3.3-V LVTTL */
	output		          		HPS_ENET_GTX_CLK,
	inout 		          		HPS_ENET_INT_N,
	output		          		HPS_ENET_MDC,
	inout 		          		HPS_ENET_MDIO,
	input 		          		HPS_ENET_RX_CLK,
	input 		     [3:0]		HPS_ENET_RX_DATA,
	input 		          		HPS_ENET_RX_DV,
	output		     [3:0]		HPS_ENET_TX_DATA,
	output		          		HPS_ENET_TX_EN,
	inout 		          		HPS_GSENSOR_INT,
	inout 		          		HPS_I2C0_SCLK,
	inout 		          		HPS_I2C0_SDAT,
	inout 		          		HPS_I2C1_SCLK,
	inout 		          		HPS_I2C1_SDAT,
	inout 		          		HPS_KEY,
	inout 		          		HPS_LED,
	inout 		          		HPS_LTC_GPIO,
	output		          		HPS_SD_CLK,
	inout 		          		HPS_SD_CMD,
	inout 		     [3:0]		HPS_SD_DATA,inout
	output		          		HPS_SPIM_CLK,
	input 		          		HPS_SPIM_MISO,
	output		          		HPS_SPIM_MOSI,
	inout 		          		HPS_SPIM_SS,
	input 		          		HPS_UART_RX,
	output		          		HPS_UART_TX,
	input 		          		HPS_USB_CLKOUT,
	inout 		     [7:0]		HPS_USB_DATA,
	input 		          		HPS_USB_DIR,
	input 		          		HPS_USB_NXT,
	output		          		HPS_USB_STP,
`endif
	
	//////////// KEY ////////////
	/* 3.3-V LVTTL */
	input				[1:0]			KEY,
	
	//////////// LED ////////////
	/* 3.3-V LVTTL */
	output	reg	[7:0]			LED,
	
	//////////// SW ////////////
	/* 3.3-V LVTTL */
	input				[3:0]			SW,

	
    output [31:0] period,
    output [31:0] duty_cycle
);

reg[31:0] counter;
reg[1:0] prev_key = 0;

wire CLOCK;
wire CLOCK2;
wire test;

//assign test = GPIO_1[35];

always @(posedge CLOCK_50) begin
		prev_key <= KEY;
//	    if ((prev_key[1] == 0 )&& (KEY[1] == 1))
//		if ( {prev_key[1], SW[1]} == 2'b01 || {prev_key[0], SW[0]} == 2'b01 )
//			LED[0] <= ~LED[0];
	end 

enum logic [1:0] {s0, s1, s2} state;

always @(posedge CLOCK_50) begin
        case (state)
            s0: begin
                counter <= 0;
                LED[0] <= 0;
                if ({prev_key[1], KEY[1]} == 2'b01) 
                    state <= s1;
                if ({prev_key[0], KEY[0]} == 2'b01) 
                    state <= s2;
               end
            s1: begin
                LED[0] <= ~LED[0];
                counter <= counter + 1;
                if (counter >= 50_000_000) 
                    state <= s2;
//                if  ({prev_key[1], KEY[1]} == 2'b01) 
//                    state <= s2;
//                if ({prev_key[0], KEY[0]} == 2'b01) 
//                    state <= s0;
               end
            s2: begin
                counter <= 0;
                LED[0] <= 1;
                if  ({prev_key[1], KEY[1]} == 2'b01)  
                    state <= s0;
                if ({prev_key[0], KEY[0]} == 2'b01) 
                    state <= s1;
               end
        endcase
        
    end
    
//	pll0 pll0_inst (
//		.refclk(CLOCK_50),   //  refclk.clk
//		.rst(),      //   reset.reset
//		.outclk_0(CLOCK),  // outclk0.clk
//		.outclk_1(CLOCK2)  // outclk1.clk
//	);
//
//	led_pio u0 (
//		.clk_clk        (CLOCK_50),        //     clk.clk
//		.reset_reset_n  (CLOCK),  //   reset.reset_n
//		.led_pio_export (LED)  // led_pio.export
//	);
	
	
//	duration_meter meter(.clock(CLOCK), .reset(0), .d(test), .q(period), .duty_cycle(duty_cycle)); 
	
endmodule
