
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


int main(void)
{
Gnuplot gp;

	std::cout << "Press Ctrl-C to quit (closing gnuplot window doesn't quit)." << std::endl;

       gp << "set autoscale xy\n";

	const int N = 1000;
	std::vector<double> pts(N);

	double theta = 0;
	while(1) {
		for(int i=0; i<N; i++) {
			double alpha = (double(i)/N-0.5) * 10;
			pts[i] = sin(alpha*8.0 + theta) * exp(-alpha*alpha/2.0);
		}

		gp << "plot '-' binary" << gp.binFmt1d(pts, "array") << "with lines notitle\n";
		gp.sendBinary1d(pts);
		gp.flush();

		theta += 0.2;
		sleep(0.01);
	}
return 1;
}


