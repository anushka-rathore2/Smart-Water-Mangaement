#include<Wire.h>
#include<ESP8266WiFi.h>
WiFiClient client;
//String distance="",temperature="",flowmeter1="",flowmeter2="",volume1="",volume2="";
const char Thing[] = "Flowmeter_left";
const char Property[] = "Flowmeter2";
 
const char Thing1[] = "Flowmeter_right";
const char Property1[] = "Flowmeter3";

const char Thing2[] = "UltrasonicSensor";
const char Property2[] = "Distance";

const char Thing3[] = "Temperature";
const char Property3[] = "Temperature";

const char Thing4[] = "Volume2";
const char Property4[] = "Volume2";

const char Thing5[] = "Volume3";
const char Property5[] = "Volume3";

const char* host = "dcs.glaitm.org"; 
const int httpsPort = 7080;
void Put(String ThingName, String ThingProperty, String Value)
  {

    Serial.println(host);
  if (!client.connect(host, httpsPort))
 {
    Serial.println("connection failed");
    return;
  } else

 {
    Serial.println("Connected to ThingWorx.");
  }
  String url = "/Thingworx/Things/" + ThingName + "/Properties/" + ThingProperty; 
  Serial.print("requesting URL: ");
  Serial.println(url);

  String strPUTReqVal = "{\"" + ThingProperty + "\":\"" + Value + "\"}";  
  Serial.print("PUT Value: ");
  Serial.println(strPUTReqVal);

  client.print(String("PUT ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "appKey: 55c13824-5b6b-428c-a15f-1dbbbf246d73" + "\r\n" +
               "x-thingworx-session: false" + "\r\n" +
               "Accept: application/json" + "\r\n" +
               "Connection: close" + "\r\n" +
               "Content-Type: application/json" + "\r\n" +
               "Content-Length: " + String(strPUTReqVal.length()) + "\r\n\r\n" +
               strPUTReqVal + "\r\n\r\n");   

  while (client.connected()) 
{
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  client.stop();
}
void setup(){
  Wire.begin(4, 5);
  Serial.begin(115200);
  WiFi.begin("realme C2","#mum&dad");
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
}

void loop(){


  Wire.requestFrom(8, 30);//0x08 = 8;
  String rec;
  String distance="",temperature="",flowmeter1="",flowmeter2="",volume1="",volume2="";
  while (0 < Wire.available())
  {
    char c = Wire.read();
    rec += c;
  }
  Serial.println(rec);
  for(int i=0;i<rec.length();i++){
    if(i<5){
      Serial.println(distance);
      distance+=rec[i];
    }
    else if(i>=5 && i<10){
      temperature+=rec[i];
    }
    else if(i>=10 && i<15){
      flowmeter1+=rec[i];
    }
    else if(i>=15 && i<20){
      volume1+=rec[i];
    }
    else if(i>=20 && i<25){
      flowmeter2+=rec[i];
    }
    else if(i>=25 && i<30){
      volume2+=rec[i];
    }
    
  }
  Serial.println(flowmeter1 + " " + flowmeter2 + " " + distance + " "+ temperature+ " "+ volume1+ " "+ volume2);
  Put(Thing,Property,flowmeter2);
  Put(Thing4,Property4,volume2);
  Put(Thing1,Property1,flowmeter1);
  Put(Thing5,Property5,volume1);
  Put(Thing2,Property2,distance);
  Put(Thing3,Property3,temperature);
  delay(1000);
}
