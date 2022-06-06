//
// Created by amir on 5/26/22.
//
#include <iostream>
#include <fcntl.h>
#include "sim_mem.h"
#include <unistd.h>

void sim_mem::print_memory() {
    int i;
    printf("\n Physical memory\n");
    for (i = 0; i < MEMORY_SIZE; i++) {
        printf("[%c]\n", main_memory[i]);
    }
}

/************************************************************************************/
void sim_mem::print_swap() {
    char *str = (char *) (malloc(this->page_size * sizeof(char)));
    int i;
    printf("\n Swap memory\n");
    lseek(swapfile_fd, 0, SEEK_SET); // go to the start of the file
    while (read(swapfile_fd, str, this->page_size) == this->page_size) {
        for (i = 0; i < page_size; i++) {
            printf("%d - [%c]\t", i, str[i]);
        }
        printf("\n");
    }
    free(str);
}

/***************************************************************************************/
void sim_mem::print_page_table() {
    int i;
    for (int j = 0; j < num_of_proc; j++) {
        printf("\n page table of process: %d \n", j);
        printf("Valid\t Dirty\t Permission \t Frame\t Swap index\n");
        for (i = 0; i < num_of_pages; i++) {
            printf("[%d]\t  |  [%d]\t|\t[%d]\t    |\t[%d]\t|\t[%d]  |\n",
                   page_table[j][i].V,
                   page_table[j][i].D,
                   page_table[j][i].P,
                   page_table[j][i].frame,
                   page_table[j][i].swap_index);
        }
    }
}

/***************************************************************************************/
sim_mem::sim_mem(char *exe_file_name1, char exe_file_name2[], char *swap_file_name, int text_size, int data_size, int bss_size,
                 int heap_stack_size, int num_of_pages, int page_size, int num_of_process) {
    this->data_size = data_size;
    this->text_size = text_size;
    this->bss_size = bss_size;
    this->heap_stack_size = heap_stack_size;
    this->num_of_pages = num_of_pages;
    this->page_size = page_size;
    this->num_of_proc = num_of_process;
    int page_T = text_size / page_size; // pages of the text
    int swapSize = page_size * (num_of_pages - page_T); // the swap size
    int page_swap = (swapSize / page_size); // pages of the swap

    page_table = new page_descriptor *[num_of_proc];
    if ((this->program_fd[0] = open(exe_file_name1, O_RDONLY)) == -1) {
        perror("could not open file");
        //destructor
        this->sim_mem::~sim_mem();
        exit(EXIT_FAILURE);
    }
    if (num_of_proc == 2 && (this->program_fd[1] = open(exe_file_name2, O_RDONLY)) == -1) {
        perror("could not open file");
        this->sim_mem::~sim_mem();
        exit(EXIT_FAILURE);
    }
    page_table[0] = new page_descriptor[num_of_pages];
    if (num_of_proc == 2) {
        page_table[1] = new page_descriptor[num_of_pages];
    }

    if ((this->swapfile_fd = open(swap_file_name, O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1) {
        cout << "error opening the swap file" << endl;
        this->sim_mem::~sim_mem();
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MEMORY_SIZE; i++) {
        main_memory[i] = '0';
    }
    for (int i = 0; i < swapSize; i++) {
        if (write(this->swapfile_fd, "0", 1) == -1) {
            perror("Write Failed");
            this->sim_mem::~sim_mem();
            exit(EXIT_FAILURE);
        }
    }
    for (int j = 0; j < num_of_process; j++)
        for (int i = 0; i < num_of_pages; i++) {
            page_table[j][i].V = 0;
            page_table[j][i].D = 0;
            page_table[j][i].frame = -1;
            page_table[j][i].swap_index = -1;
            if (i < page_T)
                page_table[j][i].P = 0;
            else
                page_table[j][i].P = 1;
        }


}

/***************************************************************************************/
sim_mem::~sim_mem() {

    if (close(swapfile_fd) == -1 || close(program_fd[0]) == -1) {
        perror("close Filed");
        exit(EXIT_FAILURE);
    }
    if (num_of_proc == 2) {
        if (close(program_fd[1]) == -1) {
            perror("close Filed");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_of_proc; i++) {
        delete[] page_table[i];
    }
    delete[] page_table;
}

/***************************************************************************************/
char sim_mem::load(int process_id, int address) {
    int index = process_id - 1;
    if (address < 0)
        return '\0';
    int page = address / page_size;
    int offset = address % page_size;
    int p_addrs;
    int pageF;
    int page_T = text_size / page_size;
    int data = data_size / page_size;

    if (page > num_of_pages) {
        fprintf(stderr, "range error\n");
        return '\0';
    }

    if (page_table[index][page].V == 1) {
        pageF = page_table[index][page].frame;
        p_addrs = (pageF * page_size) + offset;
        return main_memory[p_addrs];
    } else {
        //bss heap stack and not text.
        if (page_table[index][page].D == 0 && page_table[index][page].V == 0 && page >= (page_T + data)) {
            fprintf(stderr, "load not possible\n");
            return '\0';
        }
        if (page_table[index][page].D == 0 && page <= (page_T)) {// bring from logical memory
            char temp[page_size];
            lseek(program_fd[index], page_size * page, SEEK_SET);
            if (read(program_fd[index], temp, page_size) != page_size) {
                perror("failed to read from file\n");
                return '\0';
            }
            write_toMainMem(temp, index, page);
            pageF = page_table[index][page].frame;
            p_addrs = (pageF * page_size) + offset;
            return main_memory[p_addrs];
        }

        if (page_table[index][page].P == 1 && page < (data + page_T)) {
            char temp[page_size];
            lseek(program_fd[index], page_size * page, SEEK_SET);
            if (read(program_fd[index], temp, page_size) != page_size) {
                perror("failed to read from file\n");
                return '\0';
            }
            write_toMainMem(temp, index, page);
            pageF = page_table[index][page].frame;
            p_addrs = (pageF * page_size) + offset;
            return main_memory[p_addrs];
        }
        if (page_table[index][page].V == 0 && page_table[index][page].D == 1 && page_table[index][page].swap_index != -1) {// bring from swap memory
            char swapTemp[page];
            lseek(swapfile_fd, page_size * page_table[index][page].swap_index, SEEK_SET);
            if (read(swapfile_fd, swapTemp, page_size) != page_size) {
                perror("failed to read from swap file\n");
                return '\0';
            }
            write_toMainMem(swapTemp, index, page);
            pageF = page_table[index][page].frame;
            p_addrs = (pageF * page_size) + offset;
            return main_memory[p_addrs];
        }
    }
}

/***************************************************************************************/
void sim_mem::store(int process_id, int address, char value) {
    if (address < 0) {
        return;
    }
    int index = process_id - 1;
    int page = address / page_size;
    int offset = address % page_size;
    int p_address;
    int pageF;
    int page_T = text_size / page_size;
    int data = data_size / page_size;
    if (page > num_of_pages) {
        fprintf(stderr, "range error\n");
        return;
    }
    if (page_table[index][page].P == 0) {// no permission to write
        fprintf(stderr, "no permission to write on this page\n");
        return;
    }
    if (page_table[index][page].V == 1) {//in main memory
        pageF = page_table[index][page].frame;
        p_address = (pageF * page_size) + offset;
        main_memory[p_address] = value;
        return;
    } else if (page_table[index][page].V == 0 && page_table[index][page].D == 0 && (page < (page_T + data))) {// bring page from executable folder
        char temp[page_size];
        lseek(program_fd[index], page_size * page, SEEK_SET);
        if (read(program_fd[index], temp, page_size) != page_size) {
            perror("failed to read from executable file\n");
            return;
        }
        write_toMainMem(temp, index, page);
        pageF = page_table[index][page].frame;
        p_address = (pageF * page_size) + offset;
        main_memory[p_address] = value;//maybe change the values in the page table.
        return;
    } else if (page_table[index][page].V == 0 && page_table[index][page].D == 1) {// bring from swap file
        char swapTemp[page];
        lseek(swapfile_fd, page_size * page_table[index][page].swap_index, SEEK_SET);
        if (read(swapfile_fd, swapTemp, page_size) != page_size) {
            perror("failed to read from swap file\n");
            return;
        }
        write_toMainMem(swapTemp, index, page);
        pageF = page_table[index][page].frame;
        p_address = (pageF * page_size) + offset;
        main_memory[p_address] = value;
        return;
    } else if (page_table[index][page].V == 0 && page_table[index][page].D == 0 && (page > (page_T + data))) {//bbs heap or stack first time
        char zeros[page_size];
        for (int i = 0; i < page_size; i++) {
            zeros[i] = '0';
        }
        write_toMainMem(zeros, index, page);
        pageF = page_table[index][page].frame;
        p_address = (pageF * page_size) + offset;
        main_memory[p_address] = value;
        return;
    }


}

/**************************************************************************************/
void sim_mem::write_toMainMem(char *temp, int ind, int page) {
    int count = 0;
    int index = 0;
    int i;
    for (i = 0; i < MEMORY_SIZE; i += page_size) {
        for (int j = i; j < i + page_size; j++) {
            if (main_memory[j] == '0') {
                count++;
            }
        }
        if (count == page_size)
            break;
    }
    if (count == page_size) {
        index = i / page_size;
        page_table[ind][page].V = 1;
        page_table[ind][page].frame = index;
        que.push(index);
        //found place for the temp
        for (int j = 0; j < page_size; j++) {
            main_memory[j + (index * page_size)] = temp[j];
        }
    } else {// no space in memory write to swap file.
        char toSwap[page_size];
        int q_pop = que.front();
        int swapindex = q_pop * page_size;
        que.pop();
        int j;
        for (j= 0; j < num_of_pages; j++) {
            if (page_table[ind][j].V == 1 && page_table[ind][j].frame == q_pop) {// found the page we want to remove from the memory.
                page_table[ind][j].V = 0;
                page_table[ind][j].frame = -1;
                break;
            }
        }
        if(page_table[ind][j].D == 1) {
            page_table[ind][j].swap_index = swapindex;
            for (int a = 0; a < page_size; a++) {
                toSwap[a] = main_memory[a + (q_pop * page_size)];
            }
            lseek(swapfile_fd, swapindex, SEEK_SET);
            if (write(swapfile_fd, toSwap, page_size) == -1) {
                perror("failed to write to swap file\n");
                exit(EXIT_FAILURE);
            }
        }

        for (int a = 0; a < page_size; a++) {
            main_memory[a + (q_pop * page_size)] = temp[a];
        }
        page_table[ind][page].V = 1;
        page_table[ind][page].frame = q_pop;

        que.push(q_pop);

    }
}