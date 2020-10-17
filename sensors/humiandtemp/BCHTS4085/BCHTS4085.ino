/*
 * VDD - 최대 3.6볼트라고 했으니 아두이노에서 지원하는 3.3V 연결해준다. 
 * GND - GND
 * SCL - 아두이노 A5  ( 이건 하드웨어적으로 지정되어 있음 , 아두이노 I2C 통신 검색바람 )
 * SDA - 아두이노 A4  ( 이건 하드웨어적으로 지정되어 있음 , 아두이노 I2C 통신 검색바람 )
 * INT - 용도는 모르겠음  아래의 코드에 보면  D7 핀에 연결하면 됨 
 * 
 * 라이브러리 및 예제 주소 : https://github.com/buildit-lab/buildthing-sensors-Arduino
 * I2C 주소 변경 방법 :  링크에 나와 있긴 한데 그냥 써보면   센서 위에 보면 저항이 달려 있습니다. 현재  0x40   이저항을 떼어다가 옆에 자리  0x41에 붙이시면 됩니다. 
 *                    그리고 코딩에서 주소 부분인  #define BCHTS4085_ADDR 0x40   -> #define BCHTS4085_ADDR 0x41 로 바꾸시면 됩니다.
 * 
 */
 

#define BCHTS4085_ADDR 0x40 //HDC2010
#define ALERTPIN 7    // 센서 모듈의  INT 핀과 연결되는 아두이노 D7핀 

#include <Wire.h>   // I2C 통신을 위한 아두이노 함수 
#include <BCHTS4085.h>  //HDC2010 Temperature and Humidity Sensor  // 센서 회사에서 제공해주는 라이브러리 

void sensorInitialize(void);
void readSensorData(void);

BCHTS4085 bchts4085 (BCHTS4085_ADDR); //HDC2010 Temperature and Humidity Sensor

void setup() {
  Serial.begin (9600);

  while (!Serial) // 시리얼 모니터 열릴때까지 기다린다.(시리얼통신이 시작할때까지 )
    {
    }
    
  Wire.begin();  // i2c 통신위한것 
  sensorInitialize();  // 센서 초기화 함수 
   
}  // end of setup


void loop() {  
  readSensorData();   // 아래의 센서 읽기 함수 
  Serial.println();   // 센서 읽기 함수 끝나고 빈줄 추가 
  delay(5000);  // 5초마다 센서 읽기를 위한 딜레이  , 기본값은 1초 (1000) 였음 
}

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
 
