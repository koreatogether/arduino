

long unsigned previousTime = 0;  // 이전시간 값을 0으로 대입
long unsigned currentTime = 0;   // 현재 시간 값을 0으로 대입 

void setup() 
{
	Serial.begin(9600);
}


void loop() 
{
	currentTime = millis();  // 현재 시간 값을 밀리함수값으로 대입
	if ( Serial.available() > 0)  // 시리얼 통신 값이 0보단 큰 값이 있다면 ~
	{
		String a;  // 변수 a 를 문자열로 선언한다.
		a = Serial.readString();  // 시리얼 통신값중에 문자열이 있다면 변수 a 에 대입한다.
		if( a == "on")  // 만약 변수 a의 문자열 값이 on 이라면 ~
		{
		Serial.println( "first screen");  //  first screen을 시리얼 통신 출력을 해라
		}
				if(a == "off")		// 만약 변수 a의 문자열 값이 off 이라면 ~
				{
				Serial.println("second screen");  //  second screen을 시리얼 통신 출력을 해라
				}
					if(a == "start")  // 만약 변수 a의 문자열 값이 start 이라면 ~
					{
					Serial.println("third screen");  //  third screen을 시리얼 통신 출력을 해라
					}		
		
	}
	else  // // 시리얼 통신 값이 0보단 큰 값이 없다면 (있지 않다면 ) ~
	{
		if ( currentTime - previousTime >= 4000)  //  4000밀리초마다 실행하는 구문 ,  현재시간 값 - 이전시간값 차이가 4000보다 같거나 크다면 
		{
			Serial.println(" 123");
			previousTime = currentTime;	  // 이전 시간에 현재 시간값을 대입 하는 것으로 , 이를 통해서 다음 계산때 비교 할 수 있게 해준다.
		}
		
	}


}