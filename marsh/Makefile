all: simu tests

tests:
	make -f MakeCppUTest.mk all
	
simu:
	make -f MakeLinuxSimulator.mk all
	
clean:	
	make -f MakeCppUTest.mk clean
	make -f MakeLinuxSimulator.mk clean
	@rm -rf lib
	
