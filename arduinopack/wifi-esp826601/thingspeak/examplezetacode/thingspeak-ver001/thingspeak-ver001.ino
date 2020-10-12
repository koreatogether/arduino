/* 
본 코드는 https://m.blog.naver.com/PostView.nhn?blogId=zeta0807&logNo=221284584786&proxyReferer=http:%2F%2F203.233.19.155%2F
의 글의 코드를 그대로 가져 왔음을 밝힘입니다.

여러 코드를 둘러본결과 아래 코드는  AT COMMNAD를 사용해서 데이타를 업로드 
하는 방식을 따르는 것 같다. 

타 코드 같은 경우  ESPWIFI식의  추가 함수를 이용해서 코드를 짠것을 볼 수 있다. 

아래 코드는 공유기에 보안 모드가 없을 경우 할수 있는 경우이다. 
만약  WEP WAP 같은 암화화가 걸려 있다면 추가 코드가 필요하다 .

*/

//#if 0
	#include <SoftwareSerial.h>
	SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
								 // This means that you need to connect the TX line from the esp to the Arduino's pin 2
								 // and the RX line from the esp to the Arduino's pin 3
//	#define BAUDRATE 9600                             
//#else
	//#define esp8266 Serial3
	#define BAUDRATE 9600
//#endif

//#include <DHT.h>

#define DEBUG   true

//#define DHTPIN 4            	// what pin we're connected to
//#define DHTTYPE DHT22   		// DHT 22  (AM2302)

//#define SOIL        A0

//DHT dht(DHTPIN, DHTTYPE);

//float hum;  					//Stores humidity value
//float temp; 					//Stores temperature value
//float soil;                 	//Stores soil %
int tmp36Pin = A0;
float tmp36temperatureC;

// 하단의 코드에서 끌어다가 쓸  문자열 값 
String AP =     "U+NetBE7C";  // 공유기 SSID
String PASS =   "876C0C0BM!";	// 공유기 암호 
String WApiKey = "ENCHIMXBBMW7S8QP";		// 원하는 채널의 쓰기  전용  APIKEY값 
String RApiKey = "FUFCK320E2NM92WX";// 원하는 채널의  읽기 전용 APIKEY 값 
String HOST = "api.thingspeak.com"; //  쓰기 및 읽기 API 값들이 전송될 THINGSPEAK 접속경로  
String PORT = "80";					//  THINGSPEAK 접속 포트 번호 
String tmp36Str = "field1";
//String fieldSoil = "field1";		// 수분 센서값  를 필드 1에 배정하겠다.  만약 1번 필드 값에 보내고 싶으면  왼쪽과 같이 선언하면 된다. 
//String fieldHum  = "field2";		// 습도 값을 THINGSPEAK 채널 필드 2에 배정하겠다.
//String fieldTemp = "field3";		// 온도 값을 THINGSPEAK 채널 필드 3에 배정하겠다. 

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

void setup()  
{    
	unsigned int i=0;
	// Open Serial1 communications and wait for port to open:
	esp8266.begin(BAUDRATE);
	esp8266.setTimeout(5000);

	//dht.begin();

	Serial.begin(9600);	
	Serial.println("Thingspeak with ESP-01");



	/*
	sendCommand("AT",5,"OK");
	sendCommand("AT+CWMODE=1",5,"OK");
	sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

	//print the ip addr
	sendCommand("AT+CIFSR",2,"OK"); // get ip address
	*/
}

void loop()  
{ 
	
	tmp36();

	sendCommand("AT+CIPMUX=1",5,"OK");
	
	String getData1 = "GET /update?api_key="+ WApiKey +"&"+ tmp36Str +"="+String(tmp36temperatureC);
	sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
	sendCommand("AT+CIPSEND=0," +String(getData1.length()+4),4,">");
	esp8266.println(getData1);delay(1500);countTrueCommand++;
	sendCommand("AT+CIPCLOSE=0",5,"OK");
	
	//read soil sensor
	/*soil = (float)(analogRead(SOIL)) / 1024  * 100;
	//Serial.print("Soi sensor: ");
	//Serial.print(soil, 1);			//print .x digit	
	//Serial.println("%");

	hum = dht.readHumidity();		//read humidity
	temp= dht.readTemperature();	//read temperature
	Serial.print("Humidity: ");
	Serial.print(hum,1);			//print .x digit
	Serial.print(" %, Temp: ");
	Serial.print(temp, 1);			//print .x digit
	Serial.print(" Celsius, ");

	//THINKSPEAK에 GET을 이용하여 올리고자 하는 값을 보내는 과정  AT COMMAND 이용 
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
    */
    delay(6000);  // 이부분은 THINK 값 사이의 시간 과 일치 또는 조금 + 해서 조정 , 여기선 60초 간격으로 되어 있음 
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

 void tmp36(void)
 {
 	int readtmp36analogValue = analogRead ( tmp36Pin);

 	float voltage = readtmp36analogValue * 5.0 ;
 	voltage = voltage / 1024.0 ;

 	Serial.print(voltage );
 	Serial.println(" volts");

 	tmp36temperatureC = (voltage - 0.5) * 100 ;
 	Serial.print(tmp36temperatureC);
 	Serial.println( " degrees");

 	float tmp36temperatureF = (tmp36temperatureF * 9.0 / 5.0) + 32.0; 
 	Serial.print(tmp36temperatureF); Serial.println(" degrees F");  	
 	delay(38000);
 	}
