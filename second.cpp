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
#include <fstream>

#define SHMSZ     100
#define KEY_VALUE 5678
Gnuplot gp;
void  SIGINT_handler(int sig);
void plotChart(Gnuplot &gp,std::vector<int> valuesToPlot, std::vector<std::pair<int,int>> dataToPlot );

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
   
  //gp << "set timefmt '%d-%m %H:%M'\n";    
  //gp << "set xrange [0:100]\n";
 // gp << "set yrange [0:40]\n";
  gp << "set autoscale xy\n";
 // gp << "set xdata time\n";
 // gp << "set timefmt '%d/%m/%Y %H:%M:%S'";


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
  std::fstream ff;
  std::string line;
  std::vector<int> hours;
  std::vector<int> dates;
  std::vector<int> temp;
  std::vector<std::pair<int,int>> dataToPlot;
  ff.open("temperature.txt");
  int k=0;
  if(ff.is_open())
  {
    while(std::getline(ff,line))
    {
      std::cout <<"line: " << line <<"\n";
      //dates.push_back(std::stoi(line.substr(5,6)));
      //hours.push_back(std::stoi(line.substr(17,2))); 
      hours.push_back(k); 
      if(line.size() == 32)
      {
        temp.push_back(std::stoi(line.substr(line.size()-1,1)));
      }
      else
      {
       // std::string tempstr =  ;
       // tempstr.append("...");
        temp.push_back(std::stoi(line.substr(line.size()-2,2)));
    
      }
      // line.size() <<"\n";
      std::cout << hours[k] <<"\n";
      std::cout<<temp[k]<<"\n";
      dataToPlot.push_back(std::make_pair(hours[k], temp[k]));
      k++;
    }
  std::cout << hours.size() << " " << temp.size() << "\n";
  }
  ff.close();
  plotChart(gp, data, dataToPlot);
}

void  SIGINT_handler(int sig)
{
  signal(sig, SIG_IGN);
  printf("From SIGINT: just got a %d (SIGINT ^C) signal\n", sig);
  printData(ShmPTR);
  signal(sig, SIGINT_handler);
}

void plotChart(Gnuplot &gp, std::vector<int> valuesToPlot, std::vector<std::pair<int,int>> dataToPlot )
{
  //std::vector<std::pair<std::string,int>> data;
gp << "plot '-' using 1:2 with lines  title 'test'\n";
  //gp << "plot 'temperature.txt' using 1:3";
gp.send1d(dataToPlot);
 // gp << "plot '-' binary" << gp.binFmt1d(data, "array") << "with lines notitle\n";
  //gp.sendBinary1d(data);
  gp.flush();
}



