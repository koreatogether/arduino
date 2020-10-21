/* 
    2개의 시리얼 통신을 가진 장비를  에러 없이 코드를 돌리는게 목적입니다.
     테스트 할 부품   1. pm1001 센서  - 디웰전자 - 시리얼통신
                     2. esp8266-01 모듈 
                     3. PMSA003 센서

 디웰전자 센서 공식 홈페이지 자료실 링크입니다.
 http://diwell.com/article/%EC%9E%90%EB%A3%8C%EC%8B%A4/7/26/#none
 
 

/*****************************************************************************
*
* Copyright (C) 2016 Diwell Electronics Co.,Ltd.
* Project Name : PM1001 UART Code <softwareserial 이용>
* Version : 1.0 (2016.05.04)
* SYSTEM CLOCK : 16Mhz 
* BOARD : Arduino UNO. 5V operation 


 PORT Description

1. RX : 13  - 임의 조정          
2. TX : 11 -  임의 조정  
  먼지센서 전원은 5V로 하셔야 하며 포트 연결 방법은 회로도를 참고하십시오.

 Revision history.

1. 2016.5.4  : First version is released.
****************************************************************************

다중 소프트 시리얼에 대한 도움을 준 사이트 
https://kocoafab.cc/fboard/view/562 

https://m.blog.naver.com/PostView.nhn?blogId=roboholic84&logNo=220550737629&proxyReferer=http:%2F%2F203.233.19.155%2F 

*/


#include <PMS.h>  PMSA003용
#include <SoftwareSerial.h>


//  PM 1001 센서 기초 선언 
SoftwareSerial myserial(5, 6); 
SoftwareSerial myserial5(8, 9); 

unsigned char Send_data[4] = {0x11,0x01,0x01,0xED};       // 읽는명령
unsigned char Receive_Buff[16];                           // data buffer
unsigned long PCS;                                        // 수량 저장 변수 
float ug;                                                 // 농도 저장 변수 
unsigned char recv_cnt = 0;
// PM 1001 센서 기초 선언 끝 

PMS pms(myserial5);
PMS::DATA data;





void setup() {
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  while (!Serial) ;
  myserial.begin(9600);
  while (!myserial);
  myserial5.begin(9600);
  while (!myserial5);

  pms.passiveMode();
  pms.sleep();
}

void loop() {
myserial5.listen();  // listen () 이 선언되고 myserial5의 해당 코드를 넣는다.
 
  



	Serial.println ( " wakeuping ~~~~ ");
	pms.wakeUp();
	delay(10000);

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

        pms.sleep();
        delay(1000);


  myserial.listen();  // LISTEN() 이 선언되고 해당 코드를넣는다.
 
  Send_CMD();  // Send Read Command
  while(1)
  {
    if(myserial.available())
    { 
       Receive_Buff[recv_cnt++] = myserial.read();
      if(recv_cnt ==16){recv_cnt = 0; break;}
    }
  } 
  if(Checksum_cal() == Receive_Buff[15])  // CS 확인을 통해 통신 에러 없으면
  {
        PCS = (unsigned long)Receive_Buff[3]<<24 | (unsigned long)Receive_Buff[4]<<16 | (unsigned long)Receive_Buff[5]<<8| (unsigned long)Receive_Buff[6];  // 수량 
        ug = (float)PCS*3528/100000; // 농도 변환(이 식은 PM1001 모델만 적용됩니다.)
        Serial.write("PCS : ");
        Serial.print(PCS);
        
        Serial.write(",  ug : ");
        Serial.println(ug);
        
   }
   else
   {
     Serial.write("CHECKSUM Error");
   }
   delay(2000);       //1000ms
    
}
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

// $$$$$$$$$$$  PM1001 센서 관련 함수 시작 $$$$$$$$$$$$$$$$$

void Send_CMD(void)                                        // COMMAND
{
  unsigned char i;
  for(i=0; i<4; i++)
  {
    myserial.write(Send_data[i]);
    delay(1);      // Don't delete this line !!
  }
}
unsigned char Checksum_cal(void)                          // CHECKSUM 
{
  unsigned char count, SUM=0;
  for(count=0; count<15; count++)
  {
     SUM += Receive_Buff[count];
  }
  return 256-SUM;
}

// $$$$$$$$$$$  PM1001 센서 관련 함수 끝 $$$$$$$$$$$$$$$$$

// ###########  PMSA003 센서 관련 함수 시작 ###############



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
