

long unsigned previousTime = 0;
long unsigned currentTime = 0;

void setup() 
{
	Serial.begin(9600);
}


void loop() 
{
	currentTime = millis();
	if ( Serial.available() > 0)
	{
		String a;
		a = Serial.readString();
		if( a == "on")a
		{
		Serial.println( "first screen");
		}
				if(a == "off")		
				{
				Serial.println("second screen");
				}
					if(a == "start")
					{
					Serial.println("third screen");
					}		
		
	}
	else
	{
		if ( currentTime - previousTime >= 4000)
		{
			Serial.println(" 123");
			previousTime = currentTime;	
		}
		
	}


}