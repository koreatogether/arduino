#if 0
	#include <SoftwareSerial.h>
	SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
								 // This means that you need to connect the TX line from the esp to the Arduino's pin 2
								 // and the RX line from the esp to the Arduino's pin 3
	#define BAUDRATE 9600                             
#else
	#define esp8266 Serial3
	#define BAUDRATE 115200
#endif

#include <DHT.h>

#define DEBUG   true

#define DHTPIN 4            	// what pin we're connected to
#define DHTTYPE DHT22   		// DHT 22  (AM2302)

#define SOIL        A0

DHT dht(DHTPIN, DHTTYPE);

float hum;  					//Stores humidity value
float temp; 					//Stores temperature value
float soil;                 	//Stores soil %

String AP =     "SK_WiFiGIGA97E0";
String PASS =   "xxxxxxxxxxxxxx";
String WApiKey = "xxxxxxxxxxx";
String RApiKey = "xxxxxxxxxxxxxxxx";
String HOST = "api.thingspeak.com";
String PORT = "80";
String fieldSoil = "field1";		//soid humidity
String fieldHum  = "field2";		//Air humidity
String fieldTemp = "field3";		//Air temperature

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

void setup()  
{    
	unsigned int i=0;
	// Open Serial1 communications and wait for port to open:
	esp8266.begin(BAUDRATE);
	esp8266.setTimeout(5000);

	dht.begin();

	Serial.begin(115200);	
	Serial.println("Thingspeak with ESP-01");

	sendCommand("AT",5,"OK");
	sendCommand("AT+CWMODE=1",5,"OK");
	sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

	//print the ip addr
	sendCommand("AT+CIFSR",2,"OK"); // get ip address
}

void loop()  
{ 
	//read soil sensor
	soil = (float)(analogRead(SOIL)) / 1024  * 100;
	Serial.print("Soi sensor: ");
	Serial.print(soil, 1);			//print .x digit	
	Serial.println("%");

	hum = dht.readHumidity();		//read humidity
	temp= dht.readTemperature();	//read temperature
	Serial.print("Humidity: ");
	Serial.print(hum,1);			//print .x digit
	Serial.print(" %, Temp: ");
	Serial.print(temp, 1);			//print .x digit
	Serial.print(" Celsius, ");

	//upload to Thingspeak
	sendCommand("AT+CIPMUX=1",5,"OK");
	String getData1 = "GET /update?api_key="+ WApiKey +"&"+ fieldSoil +"="+String(soil);

	sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
	sendCommand("AT+CIPSEND=0," +String(getData1.length()+4),4,">");
	esp8266.println(getData1);delay(1500);countTrueCommand++;
	sendCommand("AT+CIPCLOSE=0",5,"OK");

	String getData2 = "GET /update?api_key="+ WApiKey +"&"+ fieldHum +"="+String(hum);
	sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
	sendCommand("AT+CIPSEND=0," +String(getData2.length()+4),4,">");
	esp8266.println(getData2);delay(1500);countTrueCommand++;
	sendCommand("AT+CIPCLOSE=0",5,"OK");

	String getData3 = "GET /update?api_key="+ WApiKey +"&"+ fieldTemp +"="+String(temp);
	sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
	sendCommand("AT+CIPSEND=0," +String(getData3.length()+4),4,">");
	esp8266.println(getData3);delay(1500);countTrueCommand++;
	sendCommand("AT+CIPCLOSE=0",5,"OK");
    
    delay(60000);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
	Serial.print(countTrueCommand);
	Serial.print(". at command => ");
	Serial.print(command);
	Serial.print(" ");
	while(countTimeCommand < (maxTime*1))
	{
		esp8266.println(command);//at+cipsend
		if(esp8266.find(readReplay))//ok
		{
			found = true;
			break;
		}

		countTimeCommand++;
	}

	if(found == true)
	{
		Serial.println("OYI");
		countTrueCommand++;
		countTimeCommand = 0;
	}

	if(found == false)
	{
		Serial.println("Fail");
		countTrueCommand = 0;
		countTimeCommand = 0;
	}

	found = false;
 }