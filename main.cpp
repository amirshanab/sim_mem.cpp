#include <iostream>
#include "sim_mem.h"
char main_memory[MEMORY_SIZE];
int main() {
    sim_mem mem_sm("exec_file.txt","","swap_file.txt",25,25,25,25,20,5,1);
    cout<<mem_sm.load(1,0)<<endl;
    cout<<mem_sm.load(1,5)<<endl;
    cout<<mem_sm.load(1,10)<<endl;
    cout<<mem_sm.load(1,15)<<endl;
    cout<<mem_sm.load(1,20)<<endl;
    cout<<mem_sm.load(1,25)<<endl;
    cout<<mem_sm.load(1,30)<<endl;
//    cout<<mem_sm.load(1,1)<<endl;
//    cout<<mem_sm.load(1,37)<<endl;
//    cout<<mem_sm.load(1,55)<<endl;
//    cout<<mem_sm.load(1,27)<<endl;
//    cout<<mem_sm.load(1,31)<<endl;
//    cout<<mem_sm.load(1,1)<<endl;
//    mem_sm.store(1,22,'x');
//    mem_sm.store(1,50,'y');
//    mem_sm.store(1,53,'y');
//    mem_sm.store(1,80,'w');
//    mem_sm.store(1,90,'z');
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    return 0;
}
