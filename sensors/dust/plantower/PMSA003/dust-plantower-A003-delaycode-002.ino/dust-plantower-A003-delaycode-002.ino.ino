


#include <PMS.h>
#include <SoftwareSerial.h>

SoftwareSerial myserial( 5, 6 );
PMS pms(myserial);
PMS::DATA data;


const int repeattime = 10000;
const int readingtime = 3000;   // 측정 간격시간 

unsigned long timerlast = 0;
unsigned long timernow = 0;
unsigned long timerbefore = 0;

unsigned long timerlast2 = 0;
unsigned long timernow2 = 0;


bool pms_power = false;
int cycles = 0;


void setup() 
{
  Serial.begin(9600);
  myserial.begin(9600);
  pms.passiveMode();
  pms.sleep(); 
  Serial.println( " sleep "); 
  

}

void loop() 
{
  // 시리얼모니터로 값을 입력받아 입력받은 값을 cycles 변수에 할당
    
    timernow2 = millis();
    if ( timernow2 - timerlast2 >= 35000)
    {   
        timerlast2 = timernow2;
        cycles = 2;
        //Serial.println(" timernow2 ");
        //Serial.println("2");        
        //Serial.println(cycles);
        //Serial.println ( " cycles is good ");
    }



  
  if ( !pms_power && cycles > 0 )
  {  
    Serial.println ( " wakeuping ~~~~ ");
    pms.wakeUp();
    timerlast = millis();
    pms_power = true ;
  }
  
   while (cycles > 0) 
     {
        timernow = millis();
        if (timernow - timerlast >= repeattime) //wakeup 상태를 repeattime에 맞게 활성화
        {
            if (timernow - timerbefore >= readingtime)   // 3초간 2번 측정 
            {
                timerbefore = timernow;
                readData();
                cycles--;
            }
        }
        //Serial.println(" cycles is  " );
        //Serial.println( cycles );
    }

     if (pms_power && cycles == 0) 
     {
        Serial.println("Going to sleep.");        
        pms.sleep();
        pms_power = false;
     }    
      //pms.sleep();
      //pms_power = false;     
}
    
    
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
