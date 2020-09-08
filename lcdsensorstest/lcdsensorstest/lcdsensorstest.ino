

// 온습도 센서 DHT22 추가  20년 9월 06일
// delay 를 없애기 위한 코드 작성  20년 9월 6일
// 0816 lcd 추가  20년 9월 6일
// 미세먼지 센서 plantower  pms a003 추가 테스트 소스출처  : https://blog.naver.com/PostView.nhn?blogId=compass1111&logNo=221283850507
// 미세먼지 센서 코드하고 온습도센서 코드하고 부딪히는것같아 여기서 커밋한번하고 분리해서 코드를 짜봐야곘음 
// 미세먼지 센서 배열하고 온습도 센서 배열하고 뒤섞이는건가 ?  ==> 밀리함수의 순서를 제대로 정해주어야 양쪽 함수가 시간대로 작동을 하는것이므로 이번에 제대로 밀리함수 순서 배울것!!
// U8glib을 활용해서 led 통해서 미세먼지 디스플레이하기 
   
#include <dht.h> // dht22용 라이브러리 출처 https://github.com/RobTillaart/DHTlib
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <PMS.h>  // by mariusz kacki
#include <U8glib.h>

#define DHT22_PIN 7 // DHT22 신호핀을 7번으로 하겠다 선언

dht DHT; // dht 를 DHT로 쓰겠다 선언

unsigned long cur_time = 0;
unsigned long pre_time = 0;
unsigned long pre_time2 = 0;
unsigned long pre_time3 = 0;
unsigned long pre_time4 = 0;

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

SoftwareSerial myserial(6, 5); // 소프트시리얼포트로 uno의 6번핀(rx) , 5번핀(tx)를 쓰겠다 선언
PMS pms(myserial);
PMS::DATA data;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // Fast I2C / TWI  

/*void draw(void){
    u8g.setFont(u8g_font_6x10);
    //u8g.setFontRefHeightExtendedText();
    //u8g.setDefaultForegroundColor();
    //u8g.setFontPosTop();
    u8g.drawStr(0,10,"hellow");
    u8g.setPrintPos(0, 20 );
    u8g.print(0,20, number);
}*/


struct // DHT22용 스트럭처
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
}

stat =
{
    0, 0, 0, 0, 0, 0, 0, 0
}; // DHT22용 스트럭처 끝

void setup()
{
    Serial.begin(115200); // 기본 시리얼 속도 115200    
    Serial.println("dht22_test.ino"); // DHT22용 setup 선언시작
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    Serial.println();
    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)"); // DHT22 용 setup 선언끝
    lcd.begin(8, 2); // 0816 lcd  8칸 / 2줄 이라고 선언
    lcd.clear(); // 0816 lcd 사용하기전 화면 청소
    myserial.begin(9600);
    pms.passiveMode();

    pre_time = millis();
    pre_time2 = pre_time;
}

void loop()
{
    
    cur_time = millis(); // delay를 없애기 위한 밀리함수 loop함수내 선언
    //cur_time2 = millis();
    if (cur_time - pre_time >= 7000)
    {
        pre_time = cur_time; // 밀리함수 끝
        // 현재 시간 - 과거시간 체크를 하는 함수 시작 및 조건 7초마다  1000 = 1초
        // DHT22 READ DATA
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
        Serial.print(DHT.humidity, 1);
        Serial.print(",\t");
        Serial.print(DHT.temperature, 1);
        Serial.print(",\t");
        Serial.print(stop - start);
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
        lcd.setCursor(0, 0); // 0816 lcd에 나타날 문구 시작
        lcd.print(DHT.temperature);
        lcd.write(B11011111); // 특수문자는 print가 아닌 write로 시작해야 한다.
        lcd.print("C");
        lcd.setCursor(0, 1);
        lcd.print(DHT.humidity);
        lcd.write(B00100101); // 0816 lcd에 나타날 문구 끝
    }
    if (cur_time - pre_time2 >= 10000)
    {
        pre_time2 = cur_time; // wakeup에 쓰일 밀리함수 2

        Serial.println("Waking up, wait 3 seconds for stable readings...");
  pms.wakeUp();
  Serial.println("Send read request...");
  pms.requestRead();

  Serial.println("Wait max. 1 second for read...");
  if (pms.readUntil(data))
  {
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);
    pms.sleep();
    
  }
  else
  {
    Serial.println("No data.");
    pms.sleep();
  }
  /*if ( cur_time - pre_time3 >= 15000)
  {
      pre_time3 = cur_time;
  Serial.println("Going to sleep for 10 seconds.");
  
  }*/
    }
    
    u8g.firstPage();
    do
    {
        u8g.setFont(u8g_font_6x10);
    //u8g.setFontRefHeightExtendedText();
    //u8g.setDefaultForegroundColor();
    //u8g.setFontPosTop();
    u8g.drawStr(0,10,"hellow");
    u8g.setPrintPos(0, 20 );
    u8g.print(data.PM_AE_UG_10_0);
    }while(u8g.nextPage());
    


}


