#!/usr/bin/env python
import urllib2, urllib, json, datetime, time, pyowm, string

i =0;
#20170704 16:00  8 5
d = 9;
temp = 0;
temp2 = 5;

t = 1;
while(i <48):
        temp2 += 4;
 	temp += 4;
	t +=1;
	i +=1;
	ff = open('abc.txt', 'a')
	if(t >24):
		t=0
		d+=1
	if(d<10):
		ff.write("2017040" + str(d) + " " + str(t) +":00" + " " +str(temp) +   " " +str(temp2) + "\n" )
	else:
		ff.write("201707" + str(d) + " " + str(t) +":00" + " " +str(temp) +   " " +str(temp2) + "\n" )
	ff.close()
	time.sleep(1)

