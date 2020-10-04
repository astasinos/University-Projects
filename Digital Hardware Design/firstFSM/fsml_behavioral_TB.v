`timescale 1ns/1ns // #1 delay corresponds to 1 ns delay and simulation will run with 1 ns accuracy.

module fsml_behavioral_TB();

	wire Dout_TB;
	reg Clock_TB, Reset_TB, Din_TB;
	// Declare design under test with the appropriate port mappings of the testbench signals.
	fsml_behavioral DUT(Dout_TB, Clock_TB, Reset_TB, Din_TB); // change this to fsml_behavioral_onehot to simulate the one hot file.
	
	// initial block to model the reset functionaliy. Reset will be active for the first 25 * 1 ns = 25 ns.
	initial
		begin
			Reset_TB = 1'b0;
		    #25 Reset_TB = 1'b1;
	end
	// Clock signal will start at zero.
	initial
		begin
			Clock_TB = 1'b0;
	end
	// And every 10 ns will be set to the inverse of itself. This way we have a period of 20 ns.
	always
		begin
			#10 Clock_TB = ~Clock_TB;
	end
	// Input signal modelling.

	initial 
		begin
			Din_TB = 1'b0; // Start at zero.
		  #5    Din_TB = 1'b1;	// Set to 1 at t = 5ns.
		  #15   Din_TB = 1'b0;	// Drop to zero at t = 20 ns. ( 15 + 5 ).
		  #35   Din_TB = 1'b1;	// Set again to 1 at t = 55 ns.
		  #10   Din_TB = 1'b0;	// Drop again at t = 65 ns.
		  #25   Din_TB = 1'b1;	// Rise again at t = 90 ns.
		  #55   Din_TB = 1'b0;	// Drop at t = 145 ns.
		  #45   Din_TB = 1'b1;	// Rise at t = 190 ns.
		  #20   Din_TB = 1'b0;	// Finally drop and remain zero at t = 210 ns.

	end

endmodule 