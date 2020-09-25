/*
출처 - 양세훈님 사이트 http://shlab001.com/
	   목록중 로터리 엔코더 코드입니다. 

20년 9월 25일 아주 저렴한 1000원짜리 로터리 엔코더 가지고 해봤는데 
      값은 나오는데 1단계 돌렸다고 생각했는데  그다음 값가지 읽어 버리는 바운싱 현상이 
      있는 것같아 보완 하거나 새로운 코드를 넣어야 할것같습니다. 


*/

int CLK = 3;
int DT = 4;
int SW = 5;

int count = 0;
int countHalf = 0;
int currentCLK;
int previousCLK;

void setup()
{
	// 모든 핀을 풀업 시켜서 테스트 1
	pinMode( CLK , INPUT );  // 오리지날 코드 
	pinMode( DT , INPUT_PULLUP );
	pinMode( SW , INPUT_PULLUP );
	Serial.begin(9600);
	previousCLK = digitalRead(CLK);	

	/*pinMode( CLK , INPUT );  // 오리지날 코드 
	pinMode( DT , INPUT );
	pinMode( SW , INPUT_PULLUP );
	Serial.begin(9600);
	previousCLK = digitalRead(CLK);
	*/
}


void loop()
{
	int T_count = digitalRead(SW);
	if (T_count == LOW)
	{
		count = 0;
	}

	
	currentCLK = digitalRead(CLK);  // 오리지날 코드 
	if (currentCLK != previousCLK)
	{
		count --;
		Serial.println(" count -- ");
			
	}
		else
		{
			Serial.println(" count ++ ");
			count ++;
			
		}
		Serial.print ( " count = " );
		Serial.println( count );	
	previousCLK = currentCLK;
	


	/*currentCLK = digitalRead(CLK);  // 오리지날 코드 
	if (currentCLK != previousCLK)
	{
		if(digitalRead(DT) != currentCLK)
		{
			count --;
			
		}
		else
		{
			count ++;
			
		}
		Serial.print ( " count = " );
		Serial.println( count );
	}
	previousCLK = currentCLK;
	*/  //오리지날 코드 끝 
}