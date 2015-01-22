#include "system.h"


void 
testReadOnlyException()
{
  FILE *fp;
  fp = fopen("testtext.txt", "r");
  fputs("Trying to write onto a read-only file", fp);

}

int
main() 
{
   testReadOnlyException();
}
