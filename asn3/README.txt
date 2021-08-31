     
Due to a complex nature of the CLion IDE makefile system, I was not able to create a custom makefile with the required execution commands. Thus, to run the program:

- Executable file generation:

gcc -o asn3 asn3.c

- Running the program:

gcc ./asn3

-----------------------------------------------------------------------------------------

I have previously made the abstract min heap data type and then used it for the creation of the priority queue, however my shortest path algorithm was not running properly likely
due to either incorrectly implemented min heap or heap queue. Thus, I decided to use the build-in C modules for those data structures. However, my output was still incorrect and I am positive that it is due to the incorrect implementation of the min heap queue, since my shortest path algorithm seems to make sense. It sort of works like it would for the undirected graph. Due to the lack of time I was not able to modify my data structure, even though I was pretty close at some point. Thus, I decided to get back to square one and submit the previous algorithm that produces some sort of logical output. Good thing is that I understood the issue and analytically know how this algorithm should be built.

