/*센서 순서는 
 * DHT 22
 * BCHTS4085
 * TMP36
 * 미세먼지센서 A003
 * 미세먼지센서 GY ~
 * 조도 센서 TEMT6000
 * 
 */
 



#include <Wire.h>   // I2C 통신을 위한 아두이노 함수 
#include <BCHTS4085.h>  // BCHTS4085 센서 회사에서 제공해주는 라이브러리 

// ************* BCHTS4085 관련 시작****************** 
#define BCHTS4085_ADDR 0x40 // BCHTS4085 센서 모듈 메모리 주소지정
#define ALERTPIN 7    // BCHTS4085 센서 모듈의  INT 핀과 연결되는 아두이노 D7핀 

void sensorInitialize(void);
void readSensorData(void);

BCHTS4085 bchts4085 (BCHTS4085_ADDR); //HDC2010 Temperature and Humidity Sensor
// ************ BCHTS4085 관련 끝  *****************

// ************ TMP36 관련 시작 ****************
int tmp36Pin = A0;
float tmp36temperatureC; 
// ************ TMP36 관련 끝 ****************


void setup() 
{
  Serial.begin (9600);  // 시리얼 통신은 9600부터 테스트 
  
  while (!Serial) // 시리얼 모니터 열릴때까지 기다린다.(시리얼통신이 시작할때까지 )
    {
    }
    
  Wire.begin();  // i2c 통신위한것 없으면 i2c 통신 안된다고 알고 있음 
  sensorInitialize();  // BCHTS4085 센서 초기화 함수 
   
}  


void loop() 
{  
  readSensorData();   // BCHTS4085 센서 읽기 함수 
  Serial.println();   // BCHTS4085센서 읽기 함수 끝나고 빈줄 추가 

  tmp36(); // tmp36센서의 값 처리 함수 호출 
  
  delay(5000);  // 5초마다 센서 읽기를 위한 딜레이  , 전체 센서에 대한 공통 딜레이 
}

// ********************************************************************
//********************  BCHTS4085  함수 시작 ***************************
void sensorInitialize(void){   // BCHTS4085 센서 초기화에 대한 함수 
  Serial.println("[BCHTS4085] Temperature & Humidity Sensor Enabled");
    
  bchts4085.setTempRes(FOURTEEN_BIT);
  bchts4085.setHumidRes(FOURTEEN_BIT);
  bchts4085.setRate(ONE_HZ);
  bchts4085.triggerMeasurement();

  Serial.println("Initialized BC Sensors Series");  
  Serial.println();
  Serial.println();
}

void readSensorData()  // BCHTS4085 센서 값을 읽고 시리얼 모니터에 출력하는 함수 
{  
  Serial.print("[BCHTS4085] Temp : ");
  Serial.print(bchts4085.readTemp());
  Serial.print(" ℃   Humid : ");
  Serial.print(bchts4085.readHumidity());         
  Serial.println(" %");
}

// ********************************************************************
//********************  BCHTS4085 센서용  함수 끝 ***************************

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&  TMP36 센서용   함수 시작 &&&&&&&&&&&&&&&&&&&&
void tmp36(void)
 {
   int readtmp36analogValue = analogRead ( tmp36Pin);

  //공급 전압이 5v 면 5.0 , 3.3v 면 3.3을 곱해준다. USB POWER = 4.68
  float voltage = readtmp36analogValue * 5.0 ;
  voltage = voltage / 1024.0 ;

  Serial.print(voltage );
  Serial.println(" volts");

  tmp36temperatureC = (voltage - 0.5) * 100 ;
    
  Serial.print(tmp36temperatureC);
  Serial.println( " degrees");

  float tmp36temperatureF = (tmp36temperatureC * 9.0 / 5.0) + 32.0; // 화씨는 여기서 직접 float로 정의 했다. 
  Serial.print(tmp36temperatureF); Serial.println(" degrees F");    
  Serial.println ( ) ; 
    delay ( 1000 ); 
 }
 // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&  TMP36 센서용   함수 끝 &&&&&&&&&&&&&&&&&&&&
