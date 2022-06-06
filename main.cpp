#include <iostream>
#include "sim_mem.h"
char main_memory[MEMORY_SIZE];
int main() {
    char val;
    char exe1[20] = "exec_file.txt";
    char exe2[20] = "exec_file2";
    char swap[20] = "swap_file.txt";
    char emp[5]="";
    sim_mem mem_sm(exe1, emp, swap, 25, 50, 25, 25, 25, 5, 1);
//    mem_sm.store(1, 98, 'X');
//    mem_sm.store(1, 105, 'y');
//    val = mem_sm.load(1, 98);
//    cout<< val<<endl;
//    val = mem_sm.load(1, 105);
//    cout<< val<<endl;
    val = mem_sm.load(1, 0);
    cout<<val<<endl;
    val = mem_sm.load(1, 5);
    cout<<val<<endl;
    val = mem_sm.load(1, 10);
    cout<<val<<endl;
    val = mem_sm.load(1, 15);
    cout<<val<<endl;
    val = mem_sm.load(1, 20);
    cout<<val<<endl;
    val = mem_sm.load(1, 25);
    cout<<val<<endl;
    val = mem_sm.load(1, 30);
    cout<<val<<endl;
    val = mem_sm.load(1, 35);
//    val = mem_sm.load(1, 120);
//    mem_sm.store(1, 98, 'X');

    cout<<val<<endl;
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();
}
