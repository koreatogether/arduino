// 데이타 시트 주소 : https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf


int tmp36Pin = A0;
float tmp36temperatureC;   // 볼트 값에 일정 식을 적용 후 변한 값이  섭씨(C) 와 화씨(F)가 된다. 

void setup()  
{    
	Serial.begin(9600);	
}

void loop()  
{ 
	
	tmp36();

}



void tmp36(void)
 {
 	int readtmp36analogValue = analogRead ( tmp36Pin);

 	float voltage = readtmp36analogValue * 5.0 ;
 	voltage = voltage / 1024.0 ;

 	Serial.print(voltage );
 	Serial.println(" volts");

 	tmp36temperatureC = (voltage - 0.5) * 100 ;
 	Serial.print(tmp36temperatureC);
 	Serial.println( " degrees");

 	float tmp36temperatureF = (tmp36temperatureF * 9.0 / 5.0) + 32.0; // 화씨는 여기서 직접 float로 정의 했다. 
 	Serial.print(tmp36temperatureF); Serial.println(" degrees F");  	
    delay ( 1000 ); 
 }

	



	

