/*
 * exectest.c
 *
 * Exec a simple program.  Return the result of Exec.
 */

#include "syscall.h"

int
main(int argc, char *argv[])	// Extra credit, test updated version of Exec
{
    int result = 1000;
    result = Exec("../test/exittest", 0, 0, 0);
    Exit(result);
	
	// bad string address
	int result2 = 1000;
	result2 = Exec("../this/is/a/bad/string/address", 0, 0, 0);
	Exit(result2);
	
	// filename DNE
	int result3 = 1000;
    result3 = Exec("../test/derp", 0, 0, 0);
    Exit(result3);
	
	// test does not end in NULL CHARACTER
	// TODO
	
	// Test addrspace; load prog that doesn't fit in memory
	// TODO
	
	// Test commandline arguments
}
