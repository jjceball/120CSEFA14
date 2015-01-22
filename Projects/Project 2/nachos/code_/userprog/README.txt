README NACHOS P2 CS120 FA14
Ricardo Cruell A09165265, Jay Ceballos A09338030, Khoa Ngyuen A09290499, Ian Rajczi A09995322
rcruell@ucsd.edu, jjceball@ucsd.edu, kgn002@ucsd.edu, irajczi@ucsd.edu

Overall project implementation:
This project involved a lot of new requirements. Since we were working a new memory management profile, an address space manager, and threads. Now since we have threads to make all of the programs run speperately they need their own address space. This is required for use in the project where abouts we make space. If all of the materials come together this would work, however in developing on the machine outside of school and moving it over to an IENG6 machine, there was a synching and making issue. After making our project mostly functional, the transfer messed everything up. Please see attatched screenshot of errors. As you can see by the readme below the program has been understood and attempted. Please see the code individually for methodology. To get it to compile we had to take out some features. As such functionality is not at its fullest, and our testing framework had to be sacrificed. Please reach out to the above email addresses with any questions. 

1 System call multi
Looking at the system call file for the header type with exec and exit we were able to determine the black box parts of the implementation. What would go in is pretty straight forward and since they were both simple returns, there was no trouble there. We worked with the AddrSpace class a lot to make sure it had everything it needed to set up the environment correctly. There were a lot of standards we came up with based on what could be heard from friends in the class to industry best practices for the size of blocks etc. Bitmap does the actual memory management so that the memory can be abstracted away from the kernel level. This made it really convenient to be able to stack multiple address spaces onto one block of memory.

2 Exec and Exit
Exec and exit are already defined by Nachos so we just had to implement the feature. Luckily since this was an interface there was some quite good documentation to help us get started. Exec needed to call start process and be provided its own environment. The result of this call returns the identifier which can be used to find its space in address can keeps the processes from getting tangled. The way they are kept from tangling each other is a lock is put in place, and this lock is used from project one. Testing this was difficult since our code from project one did not work flawlessly. In this way we had trouble finding if the errors were coming from the code we were using or creating. We needed to go back on a few things to make the test cases pass.

3.1 Exec
Simply changing the signature to allow for inputs broke almost everything we had previously written. Every call to exec now had an issued and needed to provide more arguments. Some calls to it with creating a new process was simple to figure out, others were more difficult or for a while involved just NULL. It is hard to make something thread safe and not hackable when you are taking straight command line arguments.

3.2 Copy and machine set up
Growing the address space was not trivial. To create something new we had to allocate the same amount of memory that is currently in use to a new location in memory but with an increased size of the additional space requirements. While this means that only the amount of memory needed is actually in use, it also means that if for some reason memory consumptions goes up to half and another process is added not enough space will be available and the system will fail on the memory copy.

4 read and write
There was quite a bit of documentation on this as well, since the IO is actually an interface in Nachos. We needed to create another class called SynchConsole, which basically turned the IO calls into function calls. This is good for security and future development, although not as efficient. It did provide the benefit of only having to put the error catching in one location.

5 user exceptions
Exception handling seems to be everywhere in the things that we wrote. Edge cases on all of our methods needed to be checked, although not necessarily handled like exceptions. There are recoverable bad inputs and then there are ones you must alert the user to. We had to go pull out the ASSERTS in project one because some of the errors could not be handled down there in the old code. Testing this was just using our old tests and making sure they threw exceptions or continued instead of just quitting.

6 Join
Joining fixes the part above that instead of having join with be a 0 it takes the id of the parent. This was easy to do since we were able to call on our previous work of creating a join. We passed both these methods into our join call, that is described below from project one.

7 interprocess communication
This basically means there is IO between two processes and they can work together within each others locks. There is no file IO, so this makes the work much more difficult to coordinate something, although it does make it faster since the memory is moving from the machine and not the disk. Will join from the method above now has to take more than a space id, and pipe tests.

8 multithreading:
We already had a fork and yield from project one so that after a certain amount of time or when a yield is reached there can be a switch of tasks. This is a single cycle CPU so this is really just making a focus shift. In a truly multi threaded cpu if Nachos was able to detect and pass it along to the hardware there would be gains. Although the problem is almost everything we do requires a lock which would block the other processes from being fully utilized.

Project 1 Join:
Join is a thread initiated from a parent class. When you try to join threads together a thread will wait until the child finishes everything it needs to do. Completely separate parents can never be merged, as it needs to be the same lineage. Testing this is simple, run two threads and make sure they can join and there are not resource errors

Distribution of work:
All 4 members worked on different parts and helped each other where needed. The load was relatively balanced with some putting work in earlier and others later.


