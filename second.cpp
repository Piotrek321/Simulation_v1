#include "second.h"
Gnuplot gp;

int *SharedMemoryPTR;
std::vector<int> data;
int currentLine; //To find out where are you in file

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
  pid     	   = *SharedMemoryPTR;  
 
 // gp << "set autoscale xy\n";

   
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
  getTemperatureFromFile("tmp.txt", dataToPlot);

  plotChart(gp, data, dataToPlot);
}

void getTemperatureFromFile(std::string fileName, std::vector<std::pair<int,int>> &dataToPlot)
{
  std::fstream ff, out;
  std::string line;
  int k =0;
  ff.open(fileName);
  out.open("out.txt");

  if(ff.is_open())
  {
    while(std::getline(ff,line))
    {
      //std::cout <<"line: " << line <<"\n";
      if(line[0] == 'Y') 
      {
	//std::cout <<"Y\n";
      }else
      {
       //	std::cout <<"O\n";
      }
      out <<"20170"<< (line.substr(2,1)) << "0"<<(line.substr(4,1))<< " " <<(line.substr(6,5)) <<" " << line.substr(line.size()-2,2) << "\n";
      out.flush();
      std::cout <<"Date:" << (line.substr(2,3)) <<"AAA"<< "\n";
      int temp = line.size()%2+1;
      std::cout <<"Hour:" << (line.substr(6,5)) <<"BBB"<< "\n";
      std::cout << "Temp:" << line.substr(line.size()-2,2) <<"CCC"<<'\n';
      //int temp = line.size()%2;
      //temp = std::stoi(line.substr(line.size()-1-temp,1+temp));
      //std::cout <<"temp:" << temp <<"BBB"<< "\n";
      //dataToPlot.push_back(std::make_pair(k, temp));
      k++;

    }
  }else
  {
    std::cout <<"sth wrung\n";
  }
  out.close();
  ff.close();
}

void  SIGINT_handler(int sig)
{
  signal(sig, SIG_IGN);
  printf("From SIGINT: just got a %d (SIGINT ^C) signal\n", sig);
  printData(SharedMemoryPTR);
  plotTemperature();
  signal(sig, SIGINT_handler);
}

void plotChart(Gnuplot &gp, std::vector<int> valuesToPlot, std::vector<std::pair<int,int>> dataToPlot )
{
 // gp << "plot '-' using 1:2 with lines  title 'test'\n";
//  gp.send1d(dataToPlot);
 // gp.flush();
//std::vector<std::pair<std::string,int>> data;
  gp << " set xdata time\n";
gp << "set yrange [0:22]\n";
  gp <<" set timefmt '%Y%m%d %H:M'\n";
gp << "plot [:][:][:] 'out.txt' using 1:3 with lines\n";
  //gp << "plot [:][:] 'out.txt' using 1:2\n";
//gp.send1d(dataToPlot);
gp.flush();
 // gp << "plot '-' binary" << gp.binFmt1d(data, "array") << "with lines notitle\n";
  //gp.sendBinary1d(data);

}



