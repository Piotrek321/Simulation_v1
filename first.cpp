#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <unistd.h>
#include  <stdlib.h>  
#include <iostream>
#include <vector>

//void  SIGINT_handler(int);   
void  SIGQUIT_handler(int); 

int   ShmID;      
pid_t *ShmPTR;    
pid_t anotherProcessPid;

int multipl = 1;
bool firstSignal = true;
bool shouldDataBePrepared = false;

#define SHMSZ  100
#define KEY_VALUE 5678

void signal_received(int sig, siginfo_t *info, void *context) 
{
  if(firstSignal == true) 
	{ 
 	  std::cout <<"Paring completed\n";
    anotherProcessPid = info->si_pid;
		firstSignal = false;
  }
	shouldDataBePrepared = true;
	multipl ++;
}

void writeDataToMemory(std::vector<int> data, pid_t * pointerToMemory)
{
	std::cout <<"Signal handler\n";
	int * s;         
	s = (int *)pointerToMemory;  
	*s = data.size();

	std::cout <<"s: " << s << "\n"; //To be deleted
	std::cout <<"data.size(): " << data.size() << "\n"; //To be deleted
	for (int c = 0; c <= data.size(); c++)
	{ 
		*++s = data[c];
	}
	 *s = NULL;
}

void prepareData(std::vector<int> &data, int sizeOfData)//funktor na jakas funkcje)
{
  data.clear();
  for(int i =0 ;i <=sizeOfData; i++)
	{
		data.push_back(i*multipl);
	}
}

    
int main(void)
{
    pid_t pid = getpid();
	  std::vector<int> data;
    key_t MyKey;
		MyKey = KEY_VALUE; 

		//Prepare response for signals
		struct sigaction act;
    act.sa_sigaction = &signal_received;
    act.sa_flags = SA_SIGINFO;


    sigaction(SIGINT, &act, NULL);
    if (signal(SIGQUIT, SIGQUIT_handler) == SIG_ERR) 
    {
        printf("SIGQUIT install error\n");
        exit(2);
    }
    //Prepare memory and write pid into it
 	  ShmID   = shmget(MyKey, SHMSZ, IPC_CREAT | 0666);
    ShmPTR  = (int *) shmat(ShmID, NULL, 0);
		*ShmPTR = pid; 

    for (;;) 
		{        
	 		std::cout << "Waiting for signal from another process.\n"; //To be deleted
			if(firstSignal == false)
			{
				if(shouldDataBePrepared == true)
				{	
				  prepareData(data, SHMSZ);
					writeDataToMemory(data, ShmPTR);
			  	kill(anotherProcessPid, SIGINT);
					shouldDataBePrepared = false;
				}
				
			}
        sleep(3); //To be deleted 
    }

return 1;
}

void  SIGQUIT_handler(int sig)
{
     signal(sig, SIG_IGN);
     printf("From SIGQUIT: just got a %d (SIGQUIT ^\\) signal and is about to quit\n", sig);
     shmdt(ShmPTR);
     shmctl(ShmID, IPC_RMID, NULL);
     exit(3);
}

