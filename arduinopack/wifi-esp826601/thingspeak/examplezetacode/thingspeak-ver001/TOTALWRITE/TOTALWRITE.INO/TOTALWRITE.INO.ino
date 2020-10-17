/*센서 순서는 
 * DHT 22
 * BCHTS4085
 * TMP36
 * 미세먼지센서 A003  - 코드 출처는 : https://kwonkyo.tistory.com/130 입니다.
 * 미세먼지센서 GY ~
 * 조도 센서 TEMT6000
 * 
 */

 



#include <Wire.h>   // I2C 통신을 위한 아두이노 함수 
#include <dht.h>    // dht 계열 센서중에 11 과 22를 지원하느 라이브러리
#include <BCHTS4085.h>  // BCHTS4085 센서 회사에서 제공해주는 라이브러리
#include <PMS.h>    // 미세먼지센서 PMS 시리즈에 대한 라이브러리 
#include <SoftwareSerial.h>  // 소프트시리얼 라이브러리 

unsigned long totaltimenow = 0;
unsigned long totaltimelast = 0;

// %%%%%%%%%%%%%%%%%% DHT 22 센서 관련 시작 %%%%%%%%%%%%%%%%%%
dht DHT;
#define DHT22_PIN 8                   // DHT 22 센서 모듈의 신호핀을 아두이노 D8에 배정 
struct  // 센서 검사를 위한 것이라고 하는데 이해를못함 
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
} stat = { 0,0,0,0,0,0,0,0};
// %%%%%%%%%%%%%%%%%% DHT 22 센서 관련 끝 %%%%%%%%%%%%%%%%%%

// ************* BCHTS4085 관련 시작****************** 
#define BCHTS4085_ADDR 0x40 // BCHTS4085 센서 모듈 주소지정
#define ALERTPIN 7                    // BCHTS4085 센서 모듈의  INT 핀과 연결되는 아두이노 D7핀 
void sensorInitialize(void);
void readSensorData(void);
BCHTS4085 bchts4085 (BCHTS4085_ADDR); //HDC2010 Temperature and Humidity Sensor
// ************ BCHTS4085 관련 끝  *****************

// ************ TMP36 관련 시작 ****************
int tmp36Pin = A0;
float tmp36temperatureC; 
// ************ TMP36 관련 끝 ****************

//@@@@@@@@@@@@@ PMSA003 미세먼지센서 관련 시작 @@@@@@@@@@@@@@
SoftwareSerial myserial( 5, 6 );
PMS pms(myserial);
PMS::DATA data;


const int repeattime = 10000;
const int readingtime = 3000;   // 측정 간격시간 

unsigned long timerlast = 0;
unsigned long timernow = 0;
unsigned long timerbefore = 0;

unsigned long timerlast2 = 0;
unsigned long timernow2 = 0;


bool pms_power = false;
int cycles = 0;

//=========================================================================
void setup() 
{
  Serial.begin (9600);  // 시리얼 통신은 9600부터 테스트 
  
  while (!Serial) // 시리얼 모니터 열릴때까지 기다린다.(시리얼통신이 시작할때까지 )
    {
    }
    
  Wire.begin();  // i2c 통신위한것 없으면 i2c 통신 안된다고 알고 있음 
  sensorInitialize();  // BCHTS4085 센서 초기화 함수 

  
  myserial.begin(9600);  // PMSA003 소프트시리얼 통신 속도 
  pms.passiveMode();     // 함수에서 지원하는 작동모드 중 패시브모드 
  pms.sleep();           // PMS 작동 시작을 휴면 모드로 설정 
   
}  


void loop() 
{  
  totaltimenow = millis();
  if ( totaltimenow - totaltimelast >= 30000 )
  {
  totaltimelast = totaltimenow ;
  dht22start();  // dht22 센서 읽기 함수 
  
  readSensorData();   // BCHTS4085 센서 읽기 함수 
  Serial.println();   // BCHTS4085센서 읽기 함수 끝나고 빈줄 추가 

  tmp36(); // tmp36센서의 값 처리 함수 호출 
  }
  pmsa003();  // PMS 계열 먼지센서 값 처리 함수 호출    
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
  Serial.println(" [TMP36 SENSOR]" );
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
// &&&&&&&&&&&&&&&&&&&&&&&  TMP36 센서용  루프내  함수 끝 &&&&&&&&&&&&&&&&&&&&

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%% DHT 22 센서용 루프 함수 시작 %%%%%%%%%%%%%%%%%%%
void dht22start(void) 
{
// READ DATA
    Serial.print("DHT22, \t");

    uint32_t start = micros();
    int chk = DHT.read22(DHT22_PIN);
    uint32_t stop = micros();

    stat.total++;
    switch (chk)
    {
    case DHTLIB_OK:
        stat.ok++;
        Serial.print("OK,\t");
        break;
    case DHTLIB_ERROR_CHECKSUM:
        stat.crc_error++;
        Serial.print("Checksum error,\t");
        break;
    case DHTLIB_ERROR_TIMEOUT:
        stat.time_out++;
        Serial.print("Time out error,\t");
        break;
    case DHTLIB_ERROR_CONNECT:
        stat.connect++;
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        stat.ack_l++;
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        stat.ack_h++;
        Serial.print("Ack High error,\t");
        break;
    default:
        stat.unknown++;
        Serial.print("Unknown error,\t");
        break;
    }
    // DISPLAY DATA
    Serial.println("dht22_test.ino");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)");
       Serial.print("                    ");
    Serial.print(DHT.humidity, 1);
    Serial.print(",\t");
    Serial.print("  ");
    Serial.print(DHT.temperature, 1);
    Serial.print(",\t");
    Serial.print("       ");
    Serial.print(stop - start);
    Serial.println();
    Serial.println();
    Serial.println();
    

    if (stat.total % 20 == 0)
    {
        Serial.println("\nTOT\tOK\tCRC\tTO\tCON\tACK_L\tACK_H\tUNK");
        Serial.print(stat.total);
        Serial.print("\t");
        Serial.print(stat.ok);
        Serial.print("\t");
        Serial.print(stat.crc_error);
        Serial.print("\t");
        Serial.print(stat.time_out);
        Serial.print("\t");
        Serial.print(stat.connect);
        Serial.print("\t");
        Serial.print(stat.ack_l);
        Serial.print("\t");
        Serial.print(stat.ack_h);
        Serial.print("\t");
        Serial.print(stat.unknown);
        Serial.println("\n");
    }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%% DHT 22 센서용 루프 함수 시작 %%%%%%%%%%%%%%%%%%%


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@ PMS 계열 먼지센서 루프 함수 시작 @@@@@@@@@@@@@@@@
  void readData()
    {
        PMS::DATA data;    
     
        Serial.println("Send read request...");
        pms.requestRead();
     
        Serial.println("Reading data...");
        if (pms.readUntil(data))
        {
            Serial.println();
     
            Serial.print("PM 1.0 (ug/m3): ");
            Serial.println(data.PM_AE_UG_1_0);
     
            Serial.print("PM 2.5 (ug/m3): ");
            Serial.println(data.PM_AE_UG_2_5);
     
            Serial.print("PM 10.0 (ug/m3): ");
            Serial.println(data.PM_AE_UG_10_0);
     
            Serial.println();
        }
        else
        {
            Serial.println("No data.");
        }
    }
    
 void pmsa003()
    {
      
      timernow2 = millis();
      if ( timernow2 - timerlast2 >= 35000)
      {   
          timerlast2 = timernow2;
          cycles = 1;
          //Serial.println(" timernow2 ");
          //Serial.println("2");        
          //Serial.println(cycles);
          //Serial.println ( " cycles is good ");
      }



  
      if ( !pms_power && cycles > 0 )
      {  
        Serial.println ( " wakeuping ~~~~ ");
        pms.wakeUp();
        timerlast = millis();
        pms_power = true ;
      }
  
     while (cycles > 0) 
       {
          timernow = millis();
          if (timernow - timerlast >= repeattime) //wakeup 상태를 repeattime에 맞게 활성화
          {
              if (timernow - timerbefore >= readingtime)   // 3초간 2번 측정 
              {
                  timerbefore = timernow;
                  readData();
                  cycles--;
              }
          }
          //Serial.println(" cycles is  " );
          //Serial.println( cycles );
      }

     if (pms_power && cycles == 0) 
     {
        Serial.println("Going to sleep.");        
        pms.sleep();
        pms_power = false;
     }    
      //pms.sleep();
      //pms_power = false;     
  }

    
   
    

    
