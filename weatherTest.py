#!/usr/bin/env python
import urllib2, urllib, json, datetime, time
import pyowm
import  pywapi
import string
### Yahoo
baseurl = "https://query.yahooapis.com/v1/public/yql?"
yql_query = "select * from weather.forecast where woeid=12577937 and u='c'"
yql_url = baseurl + urllib.urlencode({'q':yql_query}) + "&format=json"

result = urllib2.urlopen(yql_url).read()
data = json.loads(result)
temperature = data['query']['results']['channel']['item']['condition']['temp']

### WeatherCom
weather_com_result = pywapi.get_weather_from_weather_com('PLXX0014:1:PL')

### OWM
owm = pyowm.OWM('b5df5912a869b5cf1cfa4899b10da754')  # You MUST provide a valid API key

# Will it be sunny tomorrow at this time in Milan (Italy) ?
forecast = owm.daily_forecast("Lodz,pl")
print forecast

# Search for current weather in London (UK)
observation = owm.weather_at_place("Lodz,pl")
w = observation.get_weather()
print(w)                      

#print "AAAAAAAAAAAA"
fc = owm.three_hours_forecast('London,uk')
f = fc.get_forecast()

lst = f.get_weathers()

#for weather in f:
 # print (weather.get_reference_time('iso'),weather.get_temperature('celsius'))





print "Temperature from Yahoo: " + temperature
print "Temperature from weatherCom: " +weather_com_result['current_conditions']['temperature'] + "C now in Lodz.\n\n"
print(w.get_temperature('celsius')['temp'])             
#print "Weather.com says: It is " + string.lower(weather_com_result['current_conditions']['text']) + " and " + weather_com_result#['current_conditions']['temperature'] + "C now in New York.\n\n"
