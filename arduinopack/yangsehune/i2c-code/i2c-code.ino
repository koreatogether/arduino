/* 예를 들어  TC74XX 시리즈 가정으로 I2C 통신 하는 예임 
 본 코드는 양세훈님 자료임   사이트 http://shlab001.com
 

 */


#include <Wire.h>

int address1 = 72;  // https://youtu.be/Pe0eigi75B8 영상과 함께 보면서 해야함 
int address2 = 73;
int address3 = 74;


void setup()
{
	Serial.begin(9600);
	Wire.begin(); 	
}


void loop()
{
	int T1 = read_temp(address1);
	int T2 = read_temp(address2);
	int T3 = read_temp(address3);
	Serial.print("TC74A0:  ");
	Serial.print(T1);
	Serial.print(" C |  ");	
	Serial.println("TC74A1:  ");
	Serial.print(T2);
	Serial.print(" C |  ");
	Serial.println("TC74A2:  ");
	Serial.print(T1);
	Serial.print(" C |  ");
	delay(500);
}

int read_temp(int address)
{
	Wire.beginTransmission(address);
	Wire.write(0);
	Wire.endTransmission();

	Wire.requestFrom(address,1);
	while(Wire.available() == 0);
	int T = Wire.read();
	return T;
}



