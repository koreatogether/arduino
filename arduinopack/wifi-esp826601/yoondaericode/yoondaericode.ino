/*
설명 영상 링크  : https://youtu.be/Sq4ZXm3wMqs

팅커캐드  회로도 및 코드 : https://www.tinkercad.com/things/g3sWU8rwdoa
** 팅커 캐드 나온 화면에서 중간에 시뮬레이션 글씨 눌러서 화면 바뀐 곳에서 상단 코드 탭 
눌러야 코드 나옴 

공유기 SSID  와 접속  비밀번호 각각 3번씩 입력을 해야한다. 



*/
#include <SoftwareSerial.h> 
#define DEBUG true 
SoftwareSerial esp8266(2,3);

// 함수 millis()
 unsigned long mt1 = 0;
 unsigned long mt2 = 0;

// AT커맨드 요청시 응답받는 데이터를 시리얼 모니터에 표시
String sendCommand(String command, const int timeout, boolean debug){
 String response = "";           
 esp8266.print(command); 
 long int time = millis();
 while( (time+timeout) > millis()){
  while(esp8266.available()){        
   char c = esp8266.read(); 
    response+=c;
  }  
 }
 if(debug){
  Serial.print(response);
 }
 return response;
}

void setup(){
 Serial.begin(9600);
 esp8266.begin(9600);
 pinMode(13,OUTPUT);
 digitalWrite(13,LOW);
 
// 공유기와 자동 연결
 sendCommand("AT+RST\r\n",2000,DEBUG); 
 sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); 
 sendCommand("AT+CWJAP=\"U+NetBE7C\",\"876C0C0BM!\"\r\n",10000,DEBUG); // 공유기(와이파이) ID PW
 sendCommand("AT+CIFSR\r\n",1000,DEBUG); 
 sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG); 
 sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
 Serial.println("Server Ready");
}

void loop(){
// 1시간 마다 esp8266 전원 리셋 및 공유기와 연결 2회 (연결 안정화)
 unsigned long mt2 = millis();
 if(mt2-mt1>3599999){
  sendCommand("AT+RST\r\n",2000,DEBUG); 
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); 
  sendCommand("AT+CWJAP=\"U+NetBE7C\",\"876C0C0BM!\"\r\n",10000,DEBUG); // 공유기(와이파이) ID PW
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); 
  sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG); 
  sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
  delay(5000);
  sendCommand("AT+RST\r\n",2000,DEBUG); 
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); 
  sendCommand("AT+CWJAP=\"U+NetBE7C\",\"876C0C0BM!\"\r\n",10000,DEBUG); // 공유기(와이파이) ID PW
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); 
  sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG); 
  sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
  Serial.println("Server Ready");
  mt1=mt2;
 }
 
// Http 통신 기반으로 요청을 받았을 때 (IPD 형식으로!)
 if(esp8266.available() && (esp8266.find("+IPD,"))){
  delay(500);
  
// 요청 메세지에서 원하는 데이터를 String && int 형식으로 변환한다.
  String msg = esp8266.readStringUntil('@');
  Serial.println(msg);
  String IDstring = msg.substring(0,2); // String msg 에서 문자 3개를 추출하여, String IDstring 으로 정의할 때, 끝 자리가 \0(널) 로 인하여 없어진다. (결과:두 자리)
  Serial.println(IDstring);
  
// String 형에서 int 형으로 변환
  char IDchar[3]; // 최소 한 개 이상의 배열을 만들어야 정상 작동한다. 숫자3은 그냥 널널하게 정한 것이다. 이전 버전에서 숫자 1을 넣음으로써, 버그를 잡아 주었다. 
  IDstring.toCharArray(IDchar,IDstring.length()); // String IDstring > char IDchar[3] 변환과정에서 \0(널) 로인하여 끝자리 하나가 없어진다. (결과:한 자리)
  Serial.println(IDchar); 
  int ID = atoi(IDchar); // char 형 변수에서 숫자만 추출하여 int 형으로 반환한다. (결과:남은 한 자리 String>int)ㄴ
  Serial.println(ID); 
  
// 요청 메세지에서 원하는 한글자만 추출한다.
  String msg2 = msg.substring(11,12); // String msg 에서 문자 2개를 추출하여, String msg2 으로 정의할 때, 끝 자리가 \0(널) 로 인하여 없어진다. (결과:한 자리)
  Serial.println(msg2);

  String content;
  
// 요청 메세지에서 원하는 한글자만(문자열이다! 큰 따옴표를 이용하자!) 추출하고 이를 비교하여 원하는 동작을 실행한다.
  if(msg2 == "a"){
   digitalWrite(13,HIGH);
   content = "pin13 HIGH!"; 
  }
  else if(msg2 == "b"){
   digitalWrite(13,LOW);
   content = "pin13 LOW!";
  }
  else if(msg2 == "c"){
   int A0pinRead = analogRead(A0);
   String A0pinReadString(A0pinRead); //int형을 String형으로 변환
   content = "Cds Read: "; 
   content += A0pinReadString;
  }
 
  sendHTTPResponse(ID,content); // 아래쪽 객체함수를 순서대로 실행한다.
 }
} 

// http 통신 연결된 ID를 확인하고, Http에 의거 응답코드를 만드는 과정
void sendHTTPResponse(int ID, String content){
 String httpResponse;
 String httpHeader;
 httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n"; 
 httpHeader += "Content-Length: ";
 httpHeader += content.length();
 httpHeader += "\r\n";
 httpHeader +="Connection: close\r\n\r\n";
 httpResponse = httpHeader + content + " "; 
 
 sendCIPData(ID,httpResponse);
}

// 만들어놓은 응답코드를 AT커맨더 AT+CIPSEND= 을 통해 발송하기위해 준비한다. 발송할 데이터 길이도 측정한다.
void sendCIPData(int ID, String data){
 String cipSend = "AT+CIPSEND=";
 cipSend += ID;
 cipSend += ",";
 cipSend +=data.length();
 cipSend +="\r\n";
 sendCommand(cipSend,1000,DEBUG);
 
 sendData(data,1000,DEBUG);
}

// 이때까지 만들어놓은 데이터를 발송하고 시리얼 모니터에 발송내용을 표기한 후, 다시 응답받은 데이터를 표시한다.
String sendData(String command, const int timeout, boolean debug){
 String response = "";    
 int dataSize = command.length();
 char data[dataSize];
 command.toCharArray(data,dataSize);          
 
 esp8266.write(data,dataSize); 
 
 if(debug){
  Serial.println("\r\nResponse From Arduino");
  Serial.write(data,dataSize);
 }
 
 long int time = millis();   
 while( (time+timeout) > millis()){
  while(esp8266.available()){
   char c = esp8266.read(); // read the next character.
   response+=c;
  }  
 }
 if(debug){
  Serial.print(response);
 }    
 return response;
}