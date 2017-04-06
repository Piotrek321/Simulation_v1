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
#include  "gnuplot-iostream.h"
#include  <fstream>

#define SHMSZ     100
#define KEY_VALUE 5678

void  SIGINT_handler(int sig);
void plotChart(Gnuplot &gp,std::vector<int> valuesToPlot, std::vector<std::pair<int,int>> dataToPlot );
void getTemperatureFromFile(std::string fileName, std::vector<std::pair<int,int>> &dataToPlot);
void plotTemperature();

