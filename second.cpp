#include "second.h"
Gnuplot gp;

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
  key_t MyKey = KEY_VALUE;
  int  ShmID;
  char  line[SHMSZ], c;
  bool first = true;

  ShmID   = shmget(MyKey, SHMSZ, 0666);
  ShmPTR  = (int *) shmat(ShmID, NULL, 0);
  pid     = *ShmPTR;  
 
  gp << "set autoscale xy\n";

  //gp << "set timefmt '%d-%m %H:%M'\n";    
  //gp << "set xrange [0:100]\n";
  // gp << "set yrange [0:40]\n";
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
  int amountOfData = *s;
  data.clear();
  while(i != amountOfData)
  {
    i++;
    s++;
    std::cout<< "*s: " << (*s) << "\n" ;  //To be deleted
    data.push_back(*s);
  }
  
}

void plotTemperature()
{
  std::vector<std::pair<int,int>> dataToPlot;
  getTemperatureFromFile("temperature.txt", dataToPlot);

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
      std::cout <<"line: " << line <<"\n";
      int temp = line.size()%2;
      temp = std::stoi(line.substr(line.size()-1-temp,1+temp));
   
      dataToPlot.push_back(std::make_pair(k, temp));
      k++;

    }
  }else
  {
    std::cout <<"sth wrung\n";
  }
  ff.close();
}

void  SIGINT_handler(int sig)
{
  signal(sig, SIG_IGN);
  printf("From SIGINT: just got a %d (SIGINT ^C) signal\n", sig);
  printData(ShmPTR);
  plotTemperature();
  signal(sig, SIGINT_handler);
}

void plotChart(Gnuplot &gp, std::vector<int> valuesToPlot, std::vector<std::pair<int,int>> dataToPlot )
{
  gp << "plot '-' using 1:2 with lines  title 'test'\n";
  gp.send1d(dataToPlot);
  gp.flush();
//std::vector<std::pair<std::string,int>> data;
  //gp << "plot 'temperature.txt' using 1:3";
 // gp << "plot '-' binary" << gp.binFmt1d(data, "array") << "with lines notitle\n";
  //gp.sendBinary1d(data);

}



