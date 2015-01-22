#include "synchconsole.h"
#include "system.h"

//calls ReadDone to set variable
//since ReadDone cannot be a static function
static void doneReading (int arg){
    synchconsole = (SynchConsole *) arg;
    synchconsole->ReadDone();
}

//calls WriteDone to set variable
//Since WriteDone cannot be a static function
static void doneWriting (int arg){
   synchconsole = (SynchConsole *) arg;
   synchconsole->WriteDone();
}

//constructor initializes
SynchConsole::SynchConsole(char* in, char* out) {
    console = new Console(in, out, (VoidFunctionPtr)doneReading, (VoidFunctionPtr)doneWriting, (int)this);
    read_lock = new Lock("synch the console lock");
    read_semaphore = new Semaphore("synch the console semaphore", 0);
    write_lock = new Lock("synch the console lock");
    write_semaphore = new Semaphore("synch the console semaphore", 0);
}

//deconstructor 
SynchConsole::~SynchConsole() {
    delete read_lock;
    delete read_semaphore;
    delete write_lock;
    delete write_semaphore;
    delete console;
}


//has to lock on the variable before returning it
char SynchConsole::GetChar() {
    char val;
  
    read_lock->Acquire();
    read_semaphore->P();
    val = console->GetChar();
    read_lock->Release();

    return val;
}

//you REALLY have to have a lock before writing a variable
void SynchConsole::PutChar(char in) {

    write_lock->Acquire();
    console->PutChar(in);
    write_semaphore->P();
    write_lock->Release();
    
}

//sets the variable
void SynchConsole::ReadDone() {
    read_semaphore->V();
}

//sets the variable
void SynchConsole::WriteDone() {
    write_semaphore->V();
}


