/* pagefaultexception.c
 * 
 *	Simple program to test page fault implementation in Project 3
 *
 */

#include "syscall.h"

void
testPageFaultException()
{
  space = new AddrSpace(); 
  int faultedPNtest;

  space->InitOnFault(faultedPNtest);
}

int
main()
{
  testPageFaultException();    
}
