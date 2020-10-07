#include <SoftwareSerial.h>
 
 //날씨 정보가져오기 예제 1
#define _debug_ true
 
#define WIFI_SSID "U+NetBE7C"    //접속하고자 하는 WIFI SSID 이름
#define WIFI_PASS "876C0C0BM!"    //접속하고자 하는 WIFI 비밀번호
 
#define SERIAL_BUFFER_SIZE 512
 
bool is_wifi2 = false;
char gstring[30];
 
#define  line1  F("++++++++++++++++++++++++++++++++++++++++++++++++\r\n")
 
char reply[SERIAL_BUFFER_SIZE];
char IPDstring[6][10]= {"IPD", "SEND OK", "OK", "ERROR","NotFound",};
unsigned int connectionId = 0;
 
//HardwareSerial& esp = Serial3; 
SoftwareSerial SerialESP = SoftwareSerial(2,3);
 
void setup()
{
      Serial.begin(115200); 
      Serial.write("AT COMMAND\r\n"); 
      wifi_setup();
}
 
void loop()
{
 
}
 
 
void wifi_setup()
{
  SerialESP.begin(9600);
    
  is_wifi2 = false;
  //SendData(gstring,"AT+RST\r\n",2000,_debug_); // reset module
  SendData(gstring,"AT+CWMODE=1\r\n",1000,_debug_); // configure as access point //ESP8266_WIFI_STATION
  // Enter the SSID and password for your own network
  char buf[30];
  //"AT+CWJAP=\"myNetworkSSID\",\"password\"\r\n");
  sprintf(buf,"AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASS);
  Serial.println(buf);
  SendData(gstring, buf, 6000,_debug_); // configure as access point
  
  // get ip address
  Serial.println(F("Get the ip address assigned ny the router"));
  SendData(gstring, "AT+CIFSR\r\n",2000,_debug_); // configure as access point
 
  // parse ip address.
  char ipAddress [25];
  wifi_address(ipAddress, reply);
  Serial.println(ipAddress);
  
  _delay_ms(2000);
  return ;
}
 
 
void wifi_address(char*ip_address, char* str_address)
{
  //Serial.println(str_address);
  if (findstring(ip_address, str_address, '\"', '\"') == NULL)
  {
    strcpy(ip_address , "ERROR-ip");
    is_wifi2 = false;
  }
  else
  {
    is_wifi2 = true;
  }
  return ;
}
 
char* findstring(char* tar, char* src, char _1st, char _2st)
{
  int i, k;
  bool isfind = false;
  for(i =0, k = 0; i < strlen(src); i++)
  {
    if (isfind == false && src[i] == _1st )
    {
      isfind = true;
      continue;
    }
 
    if(isfind == true)
    {
      if(src[i] == _2st || i == strlen(src)-1)
      {
        if(i == strlen(src)-1)
        {
          tar[k] = src[i];
          k++;
        }
        
        tar[k] = '\0';
        return (src+i+1);
      }
      else
      {
        tar[k] = src[i];
        k++;
      }
    }
  }
  return NULL;
}
 
 
void SendData(char* _string, char* command, unsigned long timeout, bool debug)
{
  //memset(reply, 0x00, sizeof(char)*SERIAL_BUFFER_SIZE);
  SerialESP.print(command);
  GetReply(_string, timeout, debug);
  return ;
}
 
void  GetReply(char* _string, unsigned long timeout, bool debug)
{   
  int i=4;
  int ifind = i;
  int len = 0;
  int pos = 0;
 
  while(1)
  {
    unsigned long  itime = millis();
    while( (itime + timeout) > millis())
    {
      while(SerialESP.available() > 0 && (pos < (SERIAL_BUFFER_SIZE-1)))
      {
        char c = SerialESP.read();
        reply[pos] = c; pos++;
      }
      reply[pos] = '\0';
    }
    
    if (strlen(reply) > 0)
    {
      char* str = reply;
      for(i = 0; i < 5; i++)
      {
        char* first = strstr(str, IPDstring[i]);
        if ( first != NULL)
        {
          Serial.println( IPDstring[i]);
          if(i ==0)
          {
            char buf[150];
            sscanf(first+4, "%d,%d", &connectionId, &len);
            sprintf(buf , "-IPD = %d\r\nconnectionId = %d\r\n", len, connectionId);
            Serial.print( line1 );
            Serial.print( buf );
            Serial.print( line1 );
          }
          ifind  =  i;
          break;
        }
      }
    }
 
  
    if (strlen(reply) > 0)
    {
      Serial.print( line1 );
      Serial.println( reply);
      Serial.print( line1 );
      if (pos  == (SERIAL_BUFFER_SIZE-1))
      {
        char buf[150];
        sprintf(buf , "%d,\t%d,\tcontinue \r\n", len, pos);
        Serial.print(buf);            
        memset(reply, 0x00, sizeof(char)*SERIAL_BUFFER_SIZE);
        pos = 0;
        len = 0;
        continue;
      }
    }
    
    strcpy(_string, IPDstring[ifind]);
    return  ;
  }
  
  return ;
}
