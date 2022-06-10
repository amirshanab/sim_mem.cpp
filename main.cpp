#include <iostream>
#include "sim_mem.h"
char main_memory[MEMORY_SIZE];
int main() {
    sim_mem mem_sm("exec_file.txt","exec_file2","swap_file.txt",25,
                   25,25,25,20,5,2);
    cout<<mem_sm.load(1,0)<<endl;
    cout<<mem_sm.load(1,5)<<endl;
    cout<<mem_sm.load(1,10)<<endl;
    cout<<mem_sm.load(1,15)<<endl;
    cout<<mem_sm.load(1,20)<<endl;
    cout<<mem_sm.load(1,25)<<endl;
    cout<<mem_sm.load(1,30)<<endl;
    cout<<mem_sm.load(2,0)<<endl;
    cout<<mem_sm.load(2,5)<<endl;
    cout<<mem_sm.load(2,10)<<endl;
    cout<<mem_sm.load(2,15)<<endl;
    cout<<mem_sm.load(2,20)<<endl;
    cout<<mem_sm.load(2,25)<<endl;
    cout<<mem_sm.load(2,30)<<endl;
//    mem_sm.store(1,22,'x');
    mem_sm.store(1,50,'y');
    mem_sm.store(1,55,'y');
    mem_sm.store(1,80,'w');
    mem_sm.store(1,90,'z');
    mem_sm.store(1,95,'1');
    mem_sm.store(1,85,'5');
    mem_sm.store(1,70,'5');
    mem_sm.store(1,75,'5');
    mem_sm.store(2,50,'y');
    mem_sm.store(2,55,'y');
    mem_sm.store(2,60,'y');
    mem_sm.store(2,65,'y');
    mem_sm.store(2,80,'w');
    mem_sm.store(2,90,'z');
    mem_sm.store(2,95,'1');
    mem_sm.store(2,85,'5');
    mem_sm.store(2,70,'5');
    mem_sm.store(2,75,'5');

    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    return 0;
}
