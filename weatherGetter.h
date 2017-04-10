#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <unistd.h>
#include  <stdlib.h>  
#include <iostream>
#include <vector>

#define SHMSZ  100
#define KEY_VALUE 5678

void  SIGQUIT_handler(int); 
void writeDataToMemory(std::vector<int> data, pid_t * pointerToMemory);
void signal_received(int sig, siginfo_t *info, void *context);
void prepareDataForTest(std::vector<int> &data, int sizeOfData);


