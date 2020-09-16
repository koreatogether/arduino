/*
1. GLCD 중에 LG128643-SMLYH6V 제품이 존재함 
2. 국내 사이트 등에서 설명하는 몇가지 자료는 보통 GLCD중에 다른 칩셋을 이용하는 제품을 설명하므로 무조건 DataSheet를 보고 연결 할것 ! 
3. GLCD와 아두이노 우노 핀 연결은  U8glib 의 드라이버 설정 대로 핀을 연결할것 
    * U8glib 드라이버 초기 설정 = 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16 라고 되어있는데 이대로 하면됨 
    
1:  Vss = GND
2:  Vdd = +5V -  총 2핀이 5V 핀에 연결되어야 하기에 브레드 보드로 연결할것
3:  Vo  = LCD 볼트 운용을 위한 핀으로 데이타 시트보면 POWER supply 표에 나오는 대로 뽑으면 된다. 아래 그림 참조 
    * 출처 : 엘레파츠 https://www.eleparts.co.kr/goods/view?epx=EPXGFDHT
    * 데이타 시트 6페이지에 보면 아래의 그림의 설명이 나온다. 

                      TRIMER(10K 저항 또는 20K 저항 (추천 20K))  // 가변저항종류중에 아무거나 

              GND   -------------- V out( GLCD의 18번핀)
                           -
                           -
                           -
                           
                           Vo  ( GLCD 3번핀 )


4: RS = 아두이노 우노 17번핀 (A3핀)  // 아날로그핀은 디지털 핀으로  A0 = 14번 ~ A5 = 19번까지 디지털로 쓸수있다.
5: R/W = 아두이노 우노 16번핀 (A2핀 ) 
6: E = 아두이노 우노 18번핀 (A4핀 )
7: DB0 = 아두이노 우노 8번핀
8: DB1 = 아두이노 우노 9번핀
9: DB2 = 아두이노 우노 10번핀
10: DB3 = 아두이노 우노 11번핀
11: DB4 = 아두이노 우노 4번핀
12: DB5 = 아두이노 우노 5번핀
13: DB6 = 아두이노 우노 6번핀
14: DB7 = 아두이노 우노 7번핀
15: CS1 = 아두이노 우노 14번핀 ( A0 핀 )
16: CS2 = 아두이노 우노 15번핀 ( A1 핀 )
17: /RST = 아두이노 우노 RESET 핀 (3.3V 핀옆)  - 연결해주면 우노를 시작 또는 RESET버튼을 누를때마다 초기화해준다.
18: Vout = 가변저항의 신호선쪽에 연결해준다.
19: LEDA = +5V에 연결해주어야 한다. ( 3.3 V에 연결시 화면 안 나옴 , 이게 데이타시트 안보면 모르는 사항이다.)
20: LEDB = GND에 연결해주면된다.

마지막으로 다른 GLCD 타입은 D0 ~ D7 부분 연결안하고 나머지만 연결해서 화면글씨뜨게 하는 영상이 있는데 유튜브에 있으니
찾아서 시도 해봐도 좋은 듯 .

유튜브 다른핀 연결  영상 https://youtu.be/BulAOfyVnLs

*/

#include <U8glib.h>


U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22, "hsm Hello World!");
}



void setup (){
     // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
    
}
}


void loop()
{
 // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  //delay(50);   

}

