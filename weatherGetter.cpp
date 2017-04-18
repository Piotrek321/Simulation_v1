
#include "weatherGetter.h"
#include <json/json.h>

int   SharedMemoryID;      
pid_t *SharedMemoryPtr, anotherProcessPid;    

int multipl = 1;
bool firstSignal = true;
bool shouldDataBePrepared = false;
std::string getTemperatureFromJSON(std::stringstream &jsonData);
std::string getTemperatureFromJSON2(std::stringstream &jsonData);
int main(void)
{
  pid_t pid = getpid();
  std::vector<int> data;
  //Key should be either generated or hard coded (it must have the same value in every application that uses particular part of shared memory)
  key_t MyKey = KEY_VALUE; 

  //Prepare response for signals
  struct sigaction act;
  act.sa_sigaction = &signal_received;
  act.sa_flags = SA_SIGINFO;

  sigaction(SIGINT, &act, NULL);
  if (signal(SIGQUIT, SIGQUIT_handler) == SIG_ERR) 
  {
    printf("SIGQUIT install error\n");
    exit(2);
  }
  //Prepare memory and write pid into it
  SharedMemoryID   = shmget(MyKey, SHMSZ, IPC_CREAT | 0666);
  SharedMemoryPtr  = (int *) shmat(SharedMemoryID, NULL, 0);
  *SharedMemoryPtr = pid; 

  for (;;) 
  {        
    std::cout << "Waiting for signal from another process.\n"; //To be deleted
    if(firstSignal == false)
    {
      if(shouldDataBePrepared == true)
      {	
        prepareDataForTest(data, SHMSZ);
	writeDataToMemory(data, SharedMemoryPtr);
	kill(anotherProcessPid, SIGINT);
	shouldDataBePrepared = false;
      }
				
     }
     sleep(3); //To be deleted 
    }

return 1;
}

void  SIGQUIT_handler(int sig)
{
  signal(sig, SIG_IGN);
  printf("From SIGQUIT: just got a %d (SIGQUIT ^\\) signal and is about to quit\n", sig);
  shmdt(SharedMemoryPtr);
  shmctl(SharedMemoryID, IPC_RMID, NULL);
  exit(3);
}

void signal_received(int sig, siginfo_t *info, void *context) 
{
  if(firstSignal == true) 
  { 
    std::cout <<"Paring completed\n";
    anotherProcessPid = info->si_pid;
    firstSignal = false;
  }
  shouldDataBePrepared = true;
  multipl ++;
std::stringstream ss, ss2;

  std::ofstream outfile;

  outfile.open("test.txt", std::ios_base::app);
  outfile << "Data"; 
ss << getTemperatureFromYahoo();
//std::cout << ss.str() << "\n";
std::cout <<"Yahoo weather: " <<  getTemperatureFromJSON(ss) <<"\n";

ss2 << getTemperatureFromOWM();
//std::cout << ss.str() << "\n";
std::cout <<"OWM: "<< getTemperatureFromJSON2(ss2) <<"\n";
//getTemperatureFromOWM();
outfile.close();
}

std::string getTemperatureFromJSON(std::stringstream &jsonData)
{
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(jsonData, pt);
  //std::cout << pt.get() ;
  return pt.get<std::string>("query.results.channel.item.condition.temp");
}


std::string getTemperatureFromJSON2(std::stringstream &jsonData)
{
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(jsonData, pt);
  //std::cout << pt.get() ;
  return pt.get<std::string>("main.temp");
}


void writeDataToMemory(std::vector<int> data, pid_t * pointerToMemory)
{
	std::cout <<"Signal handler\n";
	int * s;         
	s = (int *)pointerToMemory;  
	*s = data.size();

	for (int c = 0; c <= data.size(); c++)
	{ 
	  *++s = data[c];
	}
	 *s = NULL;
}

void prepareDataForTest(std::vector<int> &data, int sizeOfData)//funktor na jakas funkcje)
{
  data.clear();
  for(int i =0 ;i <=sizeOfData; i++)
  {
    data.push_back(i*multipl);
  }
}
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    ((std::string*)userdata)->append((char*)ptr, size * nmemb);
    return size * nmemb;
}


std::string getTemperatureFromYahoo()
{
std::string data ;
	CURL * crl = curl_easy_init();
	if(crl)
	{
		curl_easy_setopt(crl, CURLOPT_URL, "https://query.yahooapis.com/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%20%3D%2012577937%20and%20u='c'&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys");
//		curl_easy_setopt(crl, CURLOPT_URL, "https://query.yahooapis.com/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%20%3D%2012577937&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys");
		curl_easy_setopt(crl, CURLOPT_HTTPGET, 1L);

 
curl_easy_setopt(crl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(crl, CURLOPT_WRITEDATA, &data);
	

		curl_easy_perform(crl);
		curl_easy_cleanup(crl);
		std::cout <<"AAA\n\n\n\n";
			//std::cout << data;	
				std::cout <<"BBB\n\n\n\n";
		return data;
	}
return "";
}

std::string getTemperatureFromOWM()
{
std::string data ;
	CURL * crl = curl_easy_init();
	if(crl)
	{
		curl_easy_setopt(crl, CURLOPT_URL, "api.openweathermap.org/data/2.5/weather?id=3093133&appid=b5df5912a869b5cf1cfa4899b10da754&units=metric"); //&mode=xml
		curl_easy_setopt(crl, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(crl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(crl, CURLOPT_WRITEDATA, &data);
		curl_easy_perform(crl);
		curl_easy_cleanup(crl);
		return data;
	}
return "";
}

