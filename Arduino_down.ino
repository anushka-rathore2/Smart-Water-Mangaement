#include <Wire.h>
#define SensorPin A1
#include <FlowMeter.h>         
unsigned long int avgValue; 
float b;
int buf[10],temp;

String str, strArd;
FlowMeter Meter1 = FlowMeter(2);
FlowMeter Meter2 = FlowMeter(3);
uint8_t period = 1000;
String a,b1,c,d,e,f;

void setup() {
   Wire.begin(8);  
  Wire.onRequest(sendEvent);
  attachInterrupt(digitalPinToInterrupt(2), Meter1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(3), Meter2ISR, RISING);
  Meter1.reset();
  Meter2.reset();
  Serial.begin(9600);
}

void loop() 
{
 
  float sensorValue = analogRead(SensorPin);
  float voltage = sensorValue * (5.0 / 1023.0); 

  for(int i=0;i<10;i++)      
  { 
    buf[i]=analogRead(A0);
    delay(10);
  }
  for(int i=0;i<9;i++)        
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      
  avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; 
  phValue=3.5*phValue;  
  

  delay(period);
  Meter1.tick(period); 
  Meter2.tick(period);

  float m1=Meter1.getCurrentFlowrate();
  float m2=Meter2.getCurrentFlowrate();
  float r1=Meter1.getTotalVolume();
  float r2=Meter2.getTotalVolume();
  //str=String(voltage)+String(phValue)+String(m1)+String(m2);
  //Serial.println("string1");
  //Serial.println(str);
 if(voltage<10) 
   a="0"+String(voltage);
  else
   a=String(voltage);
  if(phValue<10)  
   b1="0"+String(phValue);
  else
  b1=String(phValue);
  if(m1<10)
    c="0"+String(m1);
  else
   c=String(m1);
  if(r1<10) 
    d="0"+String(r1);
  else
   d=String(r1);
  if(m2<10)
  e="0"+String(m2);
  else
  e=String(m2);
  if(r2<10) 
    f="0"+String(r2);
  else
   f=String(r2);

  strArd = a+","+b1+","+c+","+d+","+e+","+f;
  str = a+b1+c+d+e+f;
  Serial.println("string2:  "+ strArd);
  delay(1000);
}
void Meter1ISR() {
Meter1.count();
}

void Meter2ISR() {

Meter2.count();
}
void sendEvent(int howmany){
  for(int i=0;i<str.length();i++)
   {
     Wire.write(str[i]);
   } 
}
