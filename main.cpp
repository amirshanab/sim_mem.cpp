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
    mem_sm.store(1,51,'y');
    mem_sm.store(1,57,'y');
    mem_sm.store(1,83,'w');
    mem_sm.store(1,94,'z');
    mem_sm.store(1,96,'1');
    mem_sm.store(1,87,'5');
    mem_sm.store(1,71,'5');
    mem_sm.store(1,78,'5');
    mem_sm.store(2,54,'y');
    mem_sm.store(2,56,'y');
    mem_sm.store(2,63,'y');
    mem_sm.store(2,66,'y');
    mem_sm.store(2,82,'w');
    mem_sm.store(2,91,'z');
    mem_sm.store(2,99,'1');
    mem_sm.store(2,86,'5');
    mem_sm.store(2,72,'5');
    mem_sm.store(2,76,'5');

    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    return 0;
}
