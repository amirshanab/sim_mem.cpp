sim_mem
authored by Amir Abu Shanab.
208586214
==Description==
sim_mem.h :-
this is the header file that contains the page table struct that holds the values of all the pages and their location.

sim_mem.cpp :-
Memory simulator written in C++, that simulates the way a memory management unit works from taking pages from the logical memory to
the main memory (RAM) and creating a swap file that is filled with pages when the main memory is full.

== Functions==
8 functions :-
1) print_memory – this function prints out the content of the main memory(RAM).
2) print_swap - this function prints out the content of the swap file.
3) print_page_table – this function prints out the content of the page table that hold the values of all the tables.
4) sim_mem – this function is the constructor of the program it initializes all the values in the header file.
5) ~sim_mem - this function is the destructor of the program it deletes every allocation done by the constructor.
6) load - this function takes in a page address and process id number, it looks for the page (in the logical memory or the ram or the swap file)
   and then it returns the value of the address inside the page.
7) store - his function takes in a page address , process id number and an int value, it does the same as the load memory but istead fof returning the value of the
    address it changes that value to the one given to the function.
8) write_toMainMem _ this function takes in the page from either the load function or the store function, and it looks for an empty space in the
    main memory to write the page in, if no empty space was found, it removes a page from the main memory and writes the new page in its place (FIFO),
    and if needed it writes the old page that was removed to a swap file.
==Program Files==
sim_mem.h
sim_mem.cpp
main.cpp
==How to compile? ==
g++ main.cpp sim_mem.cpp -o run
./run
== Input: ==
no input.
==Output: ==
the main memory.
the swap file.
the page table.S