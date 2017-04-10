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
Gnuplot gp, gp24;
void plotChart(Gnuplot &gp , Gnuplot &gp2);
int main(void)
{

while(1)
{
plotChart(gp, gp24);
sleep(1);
}

return 1;
}


void plotChart(Gnuplot &gp, Gnuplot &gp2  )
{
  gp2 << " set xdata time\n";
  gp2 <<" set timefmt '%Y%m%d %H:M'\n";
  gp2 << "set xrange [ time(0) - 86400 : time(0) ] \n";   //# 86400 sec = 1 day 
  //gp2 <<set label 1 strftime("%T",time(0)) at screen 0.5,0.5   # show the 
  //gp2 <<" set xrange ["2013-07-21 16:00":"2013-07-22 16:00"]
  gp2 << "plot 'abc.txt' using 1:3 title 'Yahoo' with lines, '' using 1:4 title 'OWM' with lines\n";
  gp2.flush();
}



