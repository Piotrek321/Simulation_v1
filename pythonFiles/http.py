#!/usr/bin/env python
import urllib2, urllib, json, datetime, time
isStart = True
start = datetime.datetime.now()

print start.hour

baseurl = "https://query.yahooapis.com/v1/public/yql?"
yql_query = "select * from weather.forecast where woeid=12577937 and u='c'"
yql_url = baseurl + urllib.urlencode({'q':yql_query}) + "&format=json"

result = urllib2.urlopen(yql_url).read()
data = json.loads(result)
date = data['query']['results']['channel']['item']['condition']['date']
tempreture = data['query']['results']['channel']['item']['condition']['temp']
print "data['query']['results']['channel']['item']['condition']['temp']: " + tempreture
print "data['query']['results']['channel']['item']['condition']['date']: " + date
print yql_url


while(1):

	time.sleep(300)
	result = urllib2.urlopen(yql_url).read()
	data = json.loads(result)
	dateTemp = data['query']['results']['channel']['item']['condition']['date']


	if(dateTemp != date):
		result = urllib2.urlopen(yql_url).read()
		data = json.loads(result)
		tempreture = data['query']['results']['channel']['item']['condition']['temp']
		date = data['query']['results']['channel']['item']['condition']['date']

		print "data['query']['results']['channel']['item']['condition']['temp']: " + tempreture
		print "data['query']['results']['channel']['item']['condition']['date']: " + date
		print "data['query']['results']['channel']['item']['condition']['text']: " + data['query']['results']['channel']['item']['condition']['text']
		print "data['query']['created']: " + data['query']['created']
		print "data['query']['results']['channel']['ttl']: " + data['query']['results']['channel']['ttl']
		print "data['query']['results']['channel']['title']: " + data['query']['results']['channel']['title']
		print "data['query']['results']['channel']['lastBuildDate']: " + data['query']['results']['channel']['lastBuildDate']
		print "data['query']['results']['channel']['item']['title']: " + data['query']['results']['channel']['item']['title']
		print "data['query']['results']['channel']['item']['pubDate']: " + data['query']['results']['channel']['item']['pubDate']

		ff = open('temperature.txt', 'a')
		ff.write(date + " "+ tempreture + "\n")
		ff.close()

		'''
		for i in range(0,10):
			print "data['query']['results']['channel']['item']['forecast'][" + str(i) +"]['date']: " +  data['query']['results']['channel']['item']['forecast'][i]['date']
			print "data['query']['results']['channel']['item']['forecast'][" + str(i) +"]['day']: " +  data['query']['results']['channel']['item']['forecast'][i]['day']
			print "data['query']['results']['channel']['item']['forecast'][" + str(i) +"]['high']: " +  data['query']['results']['channel']['item']['forecast'][i]['high']
			print "data['query']['results']['channel']['item']['forecast'][" + str(i) +"]['low']: " +  data['query']['results']['channel']['item']['forecast'][i]['low']
			print("\n")
		'''
