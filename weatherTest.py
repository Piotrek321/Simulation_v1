#!/usr/bin/env python

import urllib2, urllib, json, datetime, time
import pyowm
import string
### Yahoo
Yahoo_baseurl = "https://query.yahooapis.com/v1/public/yql?"
Yahoo_yql_query = "select * from weather.forecast where woeid=12577937 and u='c'"
Yahoo_yql_url = Yahoo_baseurl + urllib.urlencode({'q':Yahoo_yql_query}) + "&format=json"

### OWM
owm = pyowm.OWM('b5df5912a869b5cf1cfa4899b10da754')  # You MUST provide a valid API key
date = datetime.datetime.now()
print date.hour

while(1):


	Yahoo_result = urllib2.urlopen(Yahoo_yql_url).read()
	Yahoo_data = json.loads(Yahoo_result)
	Yahoo_dateTemp = Yahoo_data['query']['results']['channel']['item']['condition']['date']
	Yahoo_tempreture = Yahoo_data['query']['results']['channel']['item']['condition']['temp']
	Owm_forecast = owm.daily_forecast("Lodz,pl")
	Owm_observation = owm.weather_at_place("Lodz,pl")
	Owm_w = Owm_observation.get_weather()
	Owm_temperature = Owm_w.get_temperature('celsius')['temp']
	print "Temperature from Yahoo: " + Yahoo_tempreture
	print("Temperature from OpenWeatherM: " + str(int(Owm_temperature)))
	print "Hour: " + str(date.hour)
	ff = open('temperature.txt', 'a')
	ff.write("Y " + str(date.day) + "/" + str(date.month) + " " + str(date.hour) + ":00 " + str(Yahoo_tempreture) + "\n")
	ff.write("O " + str(date.day) + "/" + str(date.month) + " " + str(date.hour) + ":00 " + str(int(Owm_temperature)) + "\n")
	ff.close()
	time.sleep(3600)

#print forecast
#print(w)                      

#print "AAAAAAAAAAAA"
#fc = owm.three_hours_forecast('London,uk')
#f = fc.get_forecast()

#lst = f.get_weathers()

#for weather in f:
 # print (weather.get_reference_time('iso'),weather.get_temperature('celsius'))
