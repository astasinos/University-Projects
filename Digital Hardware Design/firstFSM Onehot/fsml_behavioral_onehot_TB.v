`timescale 1ns/1ns

module fsml_behavioral_onehot_TB();

	wire Dout_TB;
	reg Clock_TB, Reset_TB, Din_TB;

	fsml_behavioral DUT(Dout_TB, Clock_TB, Reset_TB, Din_TB);

	initial
		begin
			Reset_TB = 1'b0;
		    #15 Reset_TB = 1'b1;
	end

	initial
		begin
			Clock_TB = 1'b0;
	end

	always
		begin
			#10 Clock_TB = ~Clock_TB;
	end

	initial 
		begin
			Din_TB = 1'b0;
		  #35   Din_TB = 1'b1;			
		  #50   Din_TB = 1'b0;
		  #70   Din_TB = 1'b1;
		  #100   Din_TB = 1'b0;
	end

endmodule 
