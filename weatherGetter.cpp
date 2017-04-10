#include "weatherGetter.h"

int   SharedMemoryID;      
pid_t *SharedMemoryPtr, anotherProcessPid;    

int multipl = 1;
bool firstSignal = true;
bool shouldDataBePrepared = false;
 
int main(void)
{
  pid_t pid = getpid();
  std::vector<int> data;
  //Key should be either generated or hard coded (it must have the same value in every application that uses particular part of shared memory)
  key_t MyKey = KEY_VALUE; 

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
  SharedMemoryID   = shmget(MyKey, SHMSZ, IPC_CREAT | 0666);
  SharedMemoryPtr  = (int *) shmat(SharedMemoryID, NULL, 0);
  *SharedMemoryPtr = pid; 

  for (;;) 
  {        
    std::cout << "Waiting for signal from another process.\n"; //To be deleted
    if(firstSignal == false)
    {
      if(shouldDataBePrepared == true)
      {	
        prepareDataForTest(data, SHMSZ);
	writeDataToMemory(data, SharedMemoryPtr);
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
  shmdt(SharedMemoryPtr);
  shmctl(SharedMemoryID, IPC_RMID, NULL);
  exit(3);
}

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

	for (int c = 0; c <= data.size(); c++)
	{ 
	  *++s = data[c];
	}
	 *s = NULL;
}

void prepareDataForTest(std::vector<int> &data, int sizeOfData)//funktor na jakas funkcje)
{
  data.clear();
  for(int i =0 ;i <=sizeOfData; i++)
  {
    data.push_back(i*multipl);
  }
}



