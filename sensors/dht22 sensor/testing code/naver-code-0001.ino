//naver searching code <1>

//(dht 2 * 3) + arduino

//ling from : https://kin.naver.com/qna/detail.nhn?d1id=1&dirId=10401&docId=333488999&qb=ZGh0MjIg7JWE65GQ7J2064W4&enc=utf8&section=kin&rank=1&search_sort=0&spq=0

#include <DHT.h>

​

//Constants

#define DHTPIN1 7                       // 센서1

#define DHTPIN2 8                      // 센서2

#define DHTPIN3 9                      // 센서3

​

#define DHTTYPE DHT22          // DHT 22 (AM2302)

​

DHT dht1(DHTPIN1, DHTTYPE);   

DHT dht2(DHTPIN2, DHTTYPE);

DHT dht3(DHTPIN3, DHTTYPE);

​

//Variables

int chk;

float hum;                         //Stores humidity value

float temp;                       //Stores temperature value

​

void setup()

{

Serial.begin(9600);

dht.begin1();

dht.begin2();

dht.begin3();

}

​

void loop()

{

delay(2000);

​

// 센서 1 읽기 

hum = dht1.readHumidity();

temp= dht1.readTemperature();

​

// 센서1 값 시리얼 모니터에 표시 

Serial.print("Humidity 1: ");

Serial.print(hum);

Serial.print(" %, Temp 1: ");

Serial.print(temp);

Serial.println(" Celsius");

delay(1000);    

​

// 센서 2 읽기 

hum = dht2.readHumidity();

temp= dht2.readTemperature();

​

// 센서2 값 시리얼 모니터에 표시 

Serial.print("Humidity 2: ");

Serial.print(hum);

Serial.print(" %, Temp 2: ");

Serial.print(temp);

Serial.println(" Celsius");

delay(1000);    

​

// 센서 3 읽기 

hum = dht3.readHumidity();

temp= dht3.readTemperature();

​

// 센서3 값 시리얼 모니터에 표시 

Serial.print("Humidity 3: ");

Serial.print(hum);

Serial.print(" %, Temp 3: ");

Serial.print(temp);

Serial.println(" Celsius");

delay(1000);    

​

}

​
