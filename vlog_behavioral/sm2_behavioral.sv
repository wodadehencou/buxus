
module sm2_behavioral;

	`include "sm2.svh"
	`include "sm2_test.svh"
	
	initial begin
		sm2_test();
		$finish;
	end

endmodule
