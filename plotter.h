#include  <sys/shm.h>
#include  <signal.h>
#include  "gnuplot-iostream.h"

#define SHMSZ     100
#define KEY_VALUE 5678

void  SIGINT_handler(int sig);
void plotChart(Gnuplot &gp,std::vector<int> valuesToPlot, std::vector<std::pair<int,int>> dataToPlot );
void plotChart(Gnuplot &gp);

/*
void getTemperatureFromFile(std::string fileName, std::vector<std::pair<int,int>> &dataToPlot);
void plotTemperature();
*/
