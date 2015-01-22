README NACHOS P1 CS120 FA14

Group cs120f_31

Ricardo Cruell A09165265 
Jay Ceballos A09338030 
Khoa Ngyuen A09290499
Ian Rajczi A09995322

Overall Project Implementation:
There was no synchronization involved to begin with so we created our own thread class with that ties with a join. This allows for multiple items to be going “simultaneously” and it is in quotes because this is on a single processor machine. These threads have locks associated with them to make sure there are no resource jumps. This was used in our mailbox class to ensure the data structure was only modified one at a time. We then extrapolated this class to have a circular wait structure for 3 items to synchronize in a fun game of waiting for a whale

Locks and Conditions:
Locks will make sure there are resource locks, and that it will wait for the system to give it everything it needs exclusive access to. Condition is a class is there to determine what state a thread is in. It is basically a glorified flag. Lock was tested with thread tests and condition test, running through the each method usage in positive case confirmation.

Mailbox:
Mailbox is a stack that can be read and written to by multiple threads but only one at a time. It had controls on it for lock to make sure that only one way is happening. When it is empty it will block the readers, and full it will block the writer. This was very easy to test with the corner cases, check if it is empty and try to read, check if it si full try to write, and then normal operations

Join:
Join is a thread initiated from a parent class. When you try to join threads together a thread will wait until the child finishes everything it needs to do. Completely separate parents can never be merged, as it needs to be the same lineage. Testing this is simple, run two threads and make sure they can join and there are not resource errors

Priorities: 
This sets priority of threads. It can push things up in the queue of things to run. Set and get priority works with the whole threads class. Testing this needs to make sure if one thread highest priority always go before. 

Whales:
This is an extension of mailbox, but instead of waiting on a queue to be empty or full to broadcast, two resources have to be there for third one to modify. For example, there may be a ton of waiting females and matchmakers, and once the male comes along it broadcasts that it is ready. This makes everything sync up and then waits for another male once the sync is complete. Testing is like mailbox but with three items

Distribution of work:
Jay and Ian implemented code and commented. Ricardo and Khoa did all the test cases and testing methods. 