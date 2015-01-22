#include "system.h"
#include "machine.h"
#include "mipssim.h"


void
testIllegalInstrException()
{
  Instruction *ins;
  ins->opCode = 65;
  OneInstruction(ins);
   
}
int
main()
{
  testIllegalInstrException();
}
