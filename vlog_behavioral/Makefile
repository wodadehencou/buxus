default: sim

SRC = ./sm2_behavioral.sv
INCSRC = ./sm2.svh ./sm2_test.svh
TOP = sm2_behavioral

NCVLOG = ncvlog -64bit -messages -linedebug
NCELAB = ncelab -64bit -access +rwc
NCSIM = ncsim -64bit


cmp: ${SRC} ${INCSRC}
	${NCVLOG} -sv ${SRC}

elab: cmp
	${NCELAB} ${TOP}

sim: elab
	${NCSIM} ${TOP}


clean:
	$(RM) -r INCA_libs
	$(RM) -r *.log


