/*
1. GLCD 중에 LG128643-SMLYH6V 제품이 존재함 
2. 국내 사이트 등에서 설명하는 몇가지 자료는 보통 GLCD중에 다른 칩셋을 이용하는 제품을 설명하므로 무조건 DataSheet를 보고 연결 할것 ! 
3. GLCD와 아두이노 우노 핀 연결은  U8glib 의 드라이버 설정 대로 핀을 연결할것 
    * U8glib 드라이버 초기 설정 = 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16 라고 되어있는데 이대로 하면됨 

4. < 배선 설명 >    
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


4: RS(DI) = 아두이노 우노 17번핀 (A3핀)  // 아날로그핀은 디지털 핀으로  A0 = 14번 ~ A5 = 19번까지 디지털로 쓸수있다.
5: R/W = 아두이노 우노 16번핀 (A2핀 ) 
6: E(EN) = 아두이노 우노 18번핀 (A4핀 )
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

5. < V001 - 화면 구성 >
    glcd +  리미트 스위치 1 + 리미트 스위치 2번을 이용해서 다음과 같이 화면 구성을 해보고자 합니다.

    화면 1 :  로고 화면 7초간 노출 및 끝 
    화면 2 ~ 5 :  리미트 스위치 1번 , 2번의 상태상태에 따른 출력 

5-1.  ver 001 - 총평
    아래의 코드로 실행할시 의도대로 동작을 했으며 , 화면  2 ~ 5번 사이의 전환에는 충분한 화면 전환을 해주어서 마무리 됨 
    
6. <V002 - 화면 구성>
    미니 PUSH BUTTON3개를 추가하고 이를 통해서 메뉴를 전환 하고 각 메뉴에 delay를 없애고 다른 함수로 시간차를 두는 것을 목표로 하고자 합니다. 

    
*/

#include <U8glib.h>


U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16

void drawColorBox(void)
{
  u8g_uint_t w,h;
  u8g_uint_t r, g, b;
  
  w = u8g.getWidth()/32;  // 디스플레이 폭값을 32로 나눈다.
  h = u8g.getHeight()/8;  // 디스플레이 높이값을 8로 나눈다.
  for( b = 0; b < 4; b++ )
    for( g = 0; g < 8; g++ )
      for( r = 0; r < 8; r++ )
      {
        u8g.setColorIndex((r<<5) |  (g<<2) | b ); // 이건 논리인데 잘 모르겠음 
        u8g.drawBox(g*w + b*w*8, r*h, w, h);
      }
}

void drawLogo(uint8_t d)
{
#ifdef MINI_LOGO // 왜 상단이 안되고 하단이 출력이 되는걸까
    u8g.setFont(u8g_font_gdr17r);
    u8g.drawStr(0+d, 22+d, "U");
    u8g.setFont(u8g_font_gdr20n);
    u8g.drawStr90(17+d,8+d,"8");
    u8g.setFont(u8g_font_gdr17r);
    u8g.drawStr(39+d,22+d,"g");
    
    u8g.drawHLine(2+d, 25+d,34);
    u8g.drawVLine(32+d, 22+d, 12);
#else
    u8g.setFont(u8g_font_gdr25r);
    u8g.drawStr(0+d, 30+d, "U");
    u8g.setFont(u8g_font_gdr30n);
    u8g.drawStr90(23+d,10+d,"8");
    u8g.setFont(u8g_font_gdr25r);
    u8g.drawStr(53+d,30+d,"g");
    
    u8g.drawHLine(2+d, 35+d, 47);
    u8g.drawVLine(45+d, 32+d, 12);
#endif
}

void drawURL(void)
{
#ifndef MINI_LOGO  // #ifndef 는 조건처리를 위한 전처리문  if not define이라는 뜻 으로 ifdef의 반대
  u8g.setFont(u8g_font_4x6);
  if ( u8g.getHeight() < 59 )
  {
    u8g.drawStr(53,9,"code.google.com");
    u8g.drawStr(77,18,"/p/u8glib");
  }
  else
  {
    u8g.drawStr(1,54,"code.google.com/p/u8glib");
  }
#endif
}


void draw(void) {
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    drawColorBox();
  }
  u8g.setColorIndex(1);
  if ( U8G_MODE_GET_BITS_PER_PIXEL(u8g.getMode()) > 1 ) {
    drawLogo(2);
    u8g.setColorIndex(2);
    drawLogo(1);
    u8g.setColorIndex(3);
  }
  drawLogo(0);
  drawURL();
  
}

void ifdraw(void){
    u8g.setFont(u8g_font_ncenB08);
  if (digitalRead(2) == 0 && digitalRead(3) == 0 ){

  u8g.drawStr( 5 , 10 , "Door1 is close");
  u8g.drawStr( 5 , 30 , "Door2 is close");
  u8g.drawStr( 5 , 55 , "ALL Is Good!!");
  }
 
    if(digitalRead(2) == 1 && digitalRead(3) == 0){
      u8g.drawStr( 5 , 10 , "Door1 is OPEN!!~");
      u8g.drawHLine( 55 , 18 , 40);
      u8g.drawStr( 5 , 30 , "Door2 is close");
      u8g.drawStr( 5 , 55 , "Check Door1 Please");
    }
    
   
    
        if(digitalRead(2) == 0 && digitalRead(3) == 1){
          u8g.drawStr( 5 , 10 , "Door1 is close");
          u8g.drawStr( 5 , 30 , "Door2 is OPEN!!~");
          u8g.drawStr( 5 , 55 , "Check Door2 Please");
        }          
        
            if(digitalRead(2) == 1 && digitalRead(3) == 1){
              u8g.drawStr( 5 , 10 , "Door1 , 2 is OPEN!!~");              
              u8g.drawStr( 5 , 30 , "EMERGENCY !!!");
              u8g.drawStr( 5 , 50 , "Check Door !!");
              u8g.drawStr( 5 , 60 , "Hurry Up!!!!!!");
            }
}

void setup(void) {
  // flip screen, if required
  //u8g.setRot180();  
  Serial.begin(9600);
  pinMode(2 , INPUT);
  pinMode(3 , INPUT);
  u8g.firstPage();  
  do {
    draw();    
  u8g.setColorIndex(1);
  }
  while( u8g.nextPage() );
  delay(2000);
}

void loop(void) {
  
  // picture loop
  
    // rebuild the picture after some delay
  

  u8g.firstPage();
  do{
  ifdraw();
  }while ( u8g.nextPage() );


  Serial.println(digitalRead(2));
  Serial.println(digitalRead(3));
}

