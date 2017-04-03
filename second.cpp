#include  <stdio.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <signal.h>
#include  <unistd.h>
#include  <stdlib.h>  
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <map>
#include  <limits>
#include  <cmath>
#include  <cstdio>
#include  <boost/tuple/tuple.hpp>
#include  <boost/foreach.hpp>
#include "gnuplot-iostream.h"

#define SHMSZ     100
#define KEY_VALUE 5678
Gnuplot gp;
void  SIGINT_handler(int sig);
void plotChart(Gnuplot &gp,std::vector<int> valuesToPlot );

int *ShmPTR;
std::vector<int> data;
int main(void)
{

  if(signal(SIGINT, SIGINT_handler) == SIG_ERR) 
  {
     printf("SIGINT install error\n");
     exit(1);
  }
 
  pid_t   pid;
  key_t MyKey;
  MyKey = KEY_VALUE;
  int  ShmID;
  char  line[SHMSZ], c;
  bool first = true;

  ShmID   = shmget(MyKey, SHMSZ, 0666);
  ShmPTR  = (int *) shmat(ShmID, NULL, 0);
  pid     = *ShmPTR;                
  
  gp << "set autoscale xy\n";


  while (1) 
  {      

    printf("Want to interrupt  or kill another process (i or k)? \n");
    fgets(line, sizeof(line), stdin);
    c = line[0];

    if(c == 'i' || c == 'I')
    {  
       kill(pid, SIGINT);
	
       printf("Sent a SIGINT signal\n"); 
       sleep(1);   //Wait a second for signal to be received. TB deleted                  					 
    }
    else if (c == 'k' || c == 'K') 
    {
       printf("About to send a SIGQUIT signal\n");
       kill(pid, SIGQUIT);     
       printf("Done.....\n");
       exit(0);
    }
    else
      printf("Wrong keypress (%c).  Try again\n", c);
  }

return 1;
}

//Is it possible to make it template-like? Make void * not int *
void printData(int * pointerToMemory)
{
  int *s, i=0;
  s = pointerToMemory;
  std::cout <<"s: " << s << "\n"; //To be deleted
  int amountOfData = *s;
  std::cout <<"amountOfData: " << amountOfData << "\n";  //To be deleted
  data.clear();
  while(i != amountOfData)
  {
    i++;
    s++;
    std::cout<< "*s: " << (*s) << "\n" ;  //To be deleted
    data.push_back(*s);

  }

  plotChart(gp, data);
}

void  SIGINT_handler(int sig)
{
  signal(sig, SIG_IGN);
  printf("From SIGINT: just got a %d (SIGINT ^C) signal\n", sig);
  printData(ShmPTR);
  signal(sig, SIGINT_handler);
}

void plotChart(Gnuplot &gp, std::vector<int> valuesToPlot )
{
  gp << "plot '-' binary" << gp.binFmt1d(data, "array") << "with lines notitle\n";
  gp.sendBinary1d(data);
  gp.flush();
}


