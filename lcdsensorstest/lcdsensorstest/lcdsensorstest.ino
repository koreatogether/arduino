// 온습도 센서 DHT22 추가  20년 9월 06일
// delay 를 없애기 위한 코드 작성  20년 9월 6일
// 0816 lcd 추가  20년 9월 6일 
//>>> 여기서 마스터 브랜치 분리   

#include <dht.h>  // dht22용 라이브러리 출처 https://github.com/RobTillaart/DHTlib
#include <LiquidCrystal.h>

#define DHT22_PIN 7  // DHT22 신호핀을 7번으로 하겠다 선언 
dht DHT; //dht 를 DHT로 쓰겠다 선언

unsigned long cur_time = 0;  // 밀리함수 변수 선언
unsigned long pre_time = 0;
int duration = 0; // 밀리함수 변수 선언 끝 

LiquidCrystal lcd(8,9,10,11,12,13);

struct  //DHT22용 스트럭처
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
} stat = { 0,0,0,0,0,0,0,0};  //DHT22용 스트럭처 끝 

void setup() {
    
    Serial.begin(115200);  //기본 시리얼 속도 115200    
    pre_time = millis(); // delay를 없애기위한 밀리함수 setup함수내 선언     //
    Serial.println("dht22_test.ino");  //DHT22용 setup 선언시작
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    Serial.println();
    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)"); // DHT22 용 setup 선언끝
    lcd.begin(8,2); // 0816 lcd  8칸 / 2줄 이라고 선언
    lcd.clear();  // 0816 lcd 사용하기전 화면 청소 


}


void loop()
{
    cur_time = millis(); // delay를 없애기 위한 밀리함수 loop함수내 선언 
    if(cur_time - pre_time >= 7000) {  //현재 시간 - 과거시간 체크를 하는 함수 시작 및 조건 7초마다  1000 = 1초
    
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
    
    lcd.setCursor(0,0); // 0816 lcd에 나타날 문구 시작 
    lcd.print(DHT.temperature); 
    lcd.write(B11011111);    // 특수문자는 print가 아닌 write로 시작해야 한다. 
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print(DHT.humidity);
    lcd.write(B00100101); // 0816 lcd에 나타날 문구 끝 
    pre_time = cur_time;  // 밀리함수 끝 
}
}

<<<<<<< HEAD
//  서브라임이 연동되는지 체크하기 위한 줄 
=======
//웹상에서 수정하면 서브라임텍스트까지 
>>>>>>> 3c03e7b31c394983d35a753590f2be03f0f55193
    //delay(2000); // 최소 2초의 딜레이가 필요한 DHT22 센서 


