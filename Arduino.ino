#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FlowMeter.h> 

#define ONE_WIRE_BUS 5
const int trigPin = 12;
const int echoPin = 13;
int sum=0;
int count=0;
long duration;
float distance;
String str,strArd;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celcius=0;
FlowMeter Meter1 = FlowMeter(2);
FlowMeter Meter2 = FlowMeter(3);
uint8_t period = 1000;
String a,b,c,d,e,f;

void setup() {
  Wire.begin(8);  
  Wire.onRequest(sendEvent);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  sensors.begin();
  attachInterrupt(digitalPinToInterrupt(2), Meter1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(3), Meter2ISR, RISING);
  Meter1.reset();
  Meter2.reset();
  Serial.begin(9600);
   pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(7,1);
  digitalWrite(8,1);
}
int funcdis(int distance)
{
 
  sum+=distance;
  Serial.println(distance);
  count++;
  if(count==5)
  {
    int avg=sum/5;
  sum=0;
    return avg;
  }
  return 0;
}

void loop() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= float(duration*0.034/2);
  int avgr=funcdis(distance);
  
  if(distance<=36 && distance>=30)
  {  
  digitalWrite(7,0);
  digitalWrite(8,0);
  delay(6000);
  digitalWrite(7,1);
  digitalWrite(8,1);
  }
  else
  {
  digitalWrite(7,0);
  delay(6000);
  digitalWrite(7,1);
  }

  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);

  delay(period);
  Meter1.tick(period); 
  Meter2.tick(period);

  float m1=Meter1.getCurrentFlowrate();
  float m2=Meter2.getCurrentFlowrate();
  float r1=Meter1.getTotalVolume();
  float r2=Meter2.getTotalVolume();
  //str=String(distance)+String(Celcius)+String(m1)+String(m2);
  //Serial.println("string1");
  //Serial.println(str);
  if(avgr<10) 
   a="0"+String(avgr);
  else
   a=String(avgr);
  if(Celcius<10)  
   b="0"+String(Celcius);
  else
  b=String(Celcius);
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
   
if(count==5)
  {strArd = a+" "+b+" "+c+" "+d+" "+e+" "+f;
  Serial.println("string2:  "+ strArd);
   str = a+b+c+d+e+f;
  delay(6000);
  count=0;}
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
