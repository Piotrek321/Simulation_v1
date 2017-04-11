#include "plotter.h"
Gnuplot gp;

int *SharedMemoryPTR;

int main(void)
{

  if(signal(SIGINT, SIGINT_handler) == SIG_ERR) 
  {
     printf("SIGINT install error\n");
     exit(1);
  }
 
  pid_t pid;
  key_t MyKey = KEY_VALUE;
  int   SharedMemoryID;
  char  line[SHMSZ], c;
  bool  first = true;

  SharedMemoryID   = shmget(MyKey, SHMSZ, 0666);
  SharedMemoryPTR  = (int *) shmat(SharedMemoryID, NULL, 0);
  pid     	       = *SharedMemoryPTR;  

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

void  SIGINT_handler(int sig)
{
  signal(sig, SIG_IGN);
  printf("From SIGINT: just got a %d (SIGINT ^C) signal\n", sig);
  //printData(SharedMemoryPTR);
  //plotTemperature();
  signal(sig, SIGINT_handler);
}

void plotChart(Gnuplot &gp, std::vector<int> valuesToPlot, std::vector<std::pair<int,int>> dataToPlot )
{
  gp << " set xdata time\n";
  //gp << "set yrange [0:22]\n";
  gp <<" set timefmt '%Y%m%d %H:M'\n";
  gp << "plot 'abc.txt' using 1:3 title 'Yahoo' with lines, '' using 1:4 title 'OWM' with lines\n";
  gp.flush();
}


/*
//Is it possible to make it template-like? Make void * not int *
void printData(int * pointerToMemory)
{
  int *s, i=0;
  s = pointerToMemory;
  int amountOfData = *s;
  data.clear();
  while(i != amountOfData)
  {
    i++;
    s++;
    data.push_back(*s);
  }
  
}

void plotTemperature()
{
  std::vector<std::pair<int,int>> dataToPlot;
  getTemperatureFromFile("tmp.txt", dataToPlot);

  plotChart(gp, data, dataToPlot);
}




void getTemperatureFromFile(std::string fileName, std::vector<std::pair<int,int>> &dataToPlot)
{
  std::fstream ff;
  std::string line;
  int k =0;
  ff.open(fileName);

  if(ff.is_open())
  {
    while(std::getline(ff,line))
    {
     // std::cout <<"Date:" << (line.substr(2,3)) <<"AAA"<< "\n";
     // int temp = line.size()%2+1;
     // std::cout <<"Hour:" << (line.substr(6,5)) <<"BBB"<< "\n";
     // std::cout << "Temp:" << line.substr(line.size()-2,2) <<"CCC"<<'\n';
      k++;
    }
  }else
  {
    std::cout <<"sth wrung\n";
  }
  ff.close();
}
std::vector<int> data;
int currentLine; //To find out where is your current position in file
*/

