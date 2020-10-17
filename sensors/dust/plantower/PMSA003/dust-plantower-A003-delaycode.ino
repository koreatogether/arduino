// pms 라이브러리의 예제를 이해를 못해서 아래와 같이 delay로 작동하는 코드를 만들었습니다. 
// 동작 구조는  25초의 슬립 간격이 있고   깨어나서 딜레이 10초 동안 작동하고 값을 출력하는 구조입니다. 
// 개선 하려는 구조는 delay()를 안 쓰는 구조로 타 센서들과 어울리게 하는게 목적입니다. 


#include <PMS.h>  // by mariusz kacki
#include <SoftwareSerial.h>

SoftwareSerial myserial(5,6); // 소프트시리얼포트로 uno의 5번핀(rx) , 6번핀(tx)를 쓰겠다 선언 , 미세먼지 PMSA003용

PMS pms(myserial);
PMS::DATA data;

const int PMS_FOR_SLEEP_INTERVAL = 25000;
unsigned long timernow = 0;
unsigned long timerlast = 0;

void setup () 
{

  Serial.begin(9600);
  myserial.begin(9600);
  pms.passiveMode();
   pms.wakeUp();    
   //pms.sleep();
  
}

void loop()
{
  timernow = millis();

  if( (timernow - timerlast) >= PMS_FOR_SLEEP_INTERVAL) {
    timerlast = timernow;  
    Serial.println(" readdata start!!!!!!!!!!!!! ");
    readdata(); 
    Serial.println( " readdata ()   end ~~~~~~~~~~~~~~");
  }  
  else
  {
    pms.sleep();
  }
  
}


void  readdata ()
{
  PMS :: DATA data;

  // 읽기 전에 버퍼를 지 웁니다 (오래된 데이터 일 가능성이있는 데이터 제거). 일부 데이터는 수동 모드로 전환하기 전에 전송되었을 수도 있습니다.
  //while (Serial. available ()) {Serial.read (); }
  pms.wakeUp();
  delay(10000);
  Serial. println ( " Request READ ... " );
  pms.requestRead ();

  Serial.println ( "Reading DATA ...." );
  if (pms.readUntil (data))
  {
    Serial.println ();

    Serial.print ( " PM 1.0 (ug / m3) : " );
    Serial.println (data. PM_AE_UG_1_0 );

    Serial.print ( " PM 2.5 (ug / m3) : " );
    Serial.println (data. PM_AE_UG_2_5 );

    Serial.print ( " PM 10.0 (ug / m3) : " );
    Serial.println (data. PM_AE_UG_10_0 );

    Serial.println ();
  }
  else
  {
    Serial.println ( " no DATA " );
  }
}
