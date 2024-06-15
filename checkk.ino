#define BLYNK_TEMPLATE_ID "TMPL3JigGtoCH"
#define BLYNK_TEMPLATE_NAME "SmartWaterMeter"
#define BLYNK_AUTH_TOKEN "Qw-Uw2HW_wndGaR6Bwh3STeyUgeTYKys"

#ifdef ESP32
  #include <WiFi.h>
  #include <BlynkSimpleEsp32.h>
#else
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
#endif

#define BLYNK_MAX_SENDBYTES 128
#define BLYNK_PRINT Serial

char auth[] = "Qw-Uw2HW_wndGaR6Bwh3STeyUgeTYKys";
char ssid[] = "project01";
char pass[] = "project01";

#include "ThingSpeak.h"
#include <time.h>
unsigned long ch_no = 2488081;//Replace with ThingSpeak Channel number
const char * write_api = "S3EKLGFK2NRU4XFW";//Replace with ThingSpeak write API
WiFiClient client;

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define BOTtoken "6990982027:AAHO8oK7l1IloXSIQ0YrPIp9y3cFXexhbG4"
#define CHAT_ID "6266008277"
WiFiClientSecure  client1;
UniversalTelegramBot bot(BOTtoken, client1);
#define sw 23
int sw_sense;

int i=0;

// FOR FLOW SENSOR
volatile int flow_frequency;
float vol = 0.0, l_hour;// Change l_minute to l_hour
unsigned char flowsensor = 26;
unsigned long currentTime;
unsigned long cloopTime;
int sense;

//FOR DHT22 SENSOR
#include<DHT.h>
#define DHTPIN 15   // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
float h;  //Stores humidity value
float t; //Stores temperature value

//FOR PH SENSOR
const int potPin=33;
float ph;
int k = 0;

float Value=0;
// FOR TURBUDITY SENSOR
int sensorPin = 32;
float volt;
float ntu;
int j = 0;
int last = 0;

#define DIGITAL_SENSOR_PIN1 25
#define DIGITAL_SENSOR_PIN2 16
#define DIGITAL_SENSOR_PIN3 17   // soil sensor
int digitalSensorValue1;
int digitalSensorValue2;
int digitalSensorValue3;
int totalvalues = 0;
#define ANALOG_SENSOR_PIN 35    //capacitive_soil
int analogSensorValue;

#define motor 5   // pump

#define led 2   // wifi indication/running

WidgetLED indicator(V0);

void flow () {
   flow_frequency++;
}

void setup() 
{
  Serial.begin(9600);

  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  pinMode(motor, OUTPUT);
  digitalWrite(motor, HIGH);

  pinMode(potPin,INPUT);  // ph
  pinMode(sensorPin,INPUT);   //turbubity
  pinMode(flowsensor, INPUT_PULLUP);   // water
  pinMode(DIGITAL_SENSOR_PIN1, INPUT);
  pinMode(DIGITAL_SENSOR_PIN2, INPUT);
  pinMode(DIGITAL_SENSOR_PIN3, INPUT);
  pinMode(ANALOG_SENSOR_PIN, INPUT);
  pinMode(sw,INPUT_PULLUP); // leak

  currentTime = millis();
  cloopTime = currentTime;
  attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING);
   
  Serial.println("Temperature and Humidity Sensor Test");
   
  Serial.println(" * READY * ");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  client1.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

    
  Blynk.begin(auth,ssid, pass);

  ThingSpeak.begin(client);

  dht.begin();

  // client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  bot.sendMessage(CHAT_ID, "kit ready");
}

void loop () 
{
   
  soil_sensor_data();

  water_flow_data();
  
  ph_data();

  turbudity();

  dht_data();

  senddata();

  Blynk.virtualWrite(V1,totalvalues);
  Blynk.virtualWrite(V2,l_hour);
  Blynk.virtualWrite(V3,vol);
  indicator.on();delay(200);indicator.off();delay(300);
  Blynk.run(); // Initiates Blynk
  delay(1000);

  leak_detect();
}
void water_flow_data()
{
  currentTime = millis();

  if(currentTime >= (cloopTime + 1000))
  {
    cloopTime = currentTime; // Updates cloopTime

    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency / 7.5) * 60; // Convert flow rate to L/hour
      vol += (l_hour / 3600); // Accumulate volume in liters per second (1 hour = 3600 seconds)
      flow_frequency = 0; // Reset Counter
      Serial.print("flow rate : ");
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/Hour");
    }
    else {
      l_hour = 0;
      Serial.println("flow rate = 0 ");
    }
    Serial.print("volume : ");
    Serial.print(vol, DEC); // Print total volume flowed in liters
    Serial.println(" L");
   }
}

void senddata()
{
  ThingSpeak.setField(1, ph);
  ThingSpeak.setField(2, volt);
  ThingSpeak.setField(3, ntu);
  ThingSpeak.setField(4, vol);
  ThingSpeak.setField(5, h);
  ThingSpeak.setField(6, t); 
  ThingSpeak.setField(7, totalvalues);
  ThingSpeak.setField(8, analogSensorValue);
  ThingSpeak.writeFields(ch_no, write_api);
}


void ph_data()
{
  Value= analogRead(potPin);
  // Serial.print(Value);
  // Serial.print(" | ");
  float voltage=Value*(3.3/4095.0);
  ph=(3.3*voltage);
  Serial.print("pH value: ");
  Serial.println(ph);
  delay(500);
}

void turbudity()
{
  volt = 0;
  for(int i=0; i<800; i++)
  {
    volt += ((float)analogRead(sensorPin)/4095)*3.3;
  }
  volt = volt/800;
  volt = round_to_dp(volt,2);
  if(volt < 2.5)
  {
    ntu = 3000;
  }
  else
  {
    ntu = -1120.4*sq(volt)+5742.3*volt-4353.8; 
  }
  Serial.print("Turbidity value: ");
  Serial.println(volt);
  Serial.print("NTU levels: ");
  Serial.println(ntu);
  delay(2000);
}

void leak_detect()
{
  sw_sense=digitalRead(sw);
  sw_sense=1-sw_sense;

  if(sw_sense==1)
  {
    i=i+1;
    Serial.println("leak detected");
    if(i==1)
    {
      bot.sendMessage(CHAT_ID, "leak detected");
    }
    //message
  }
  else
  {
    i=0;
    Serial.println("normal");
  }
}                                         
                                           
void soil_sensor_data()
{
  digitalSensorValue1 = digitalRead(DIGITAL_SENSOR_PIN1);
  digitalSensorValue1 =1-digitalSensorValue1;
  digitalSensorValue2 = digitalRead(DIGITAL_SENSOR_PIN2);
  digitalSensorValue2 =1-digitalSensorValue2;
  digitalSensorValue3 = digitalRead(DIGITAL_SENSOR_PIN3);
  digitalSensorValue3 =1-digitalSensorValue3;
  Serial.print("Digital Sensor Value1: ");Serial.println(digitalSensorValue1);
  Serial.print("Digital Sensor Value2: ");Serial.println(digitalSensorValue2);
  Serial.print("Digital Sensor Value3: ");Serial.println(digitalSensorValue3);
  totalvalues = digitalSensorValue1+digitalSensorValue2+digitalSensorValue3;

  analogSensorValue = analogRead(ANALOG_SENSOR_PIN);
  analogSensorValue =map(analogSensorValue,0,4096,0,100);
  Serial.print("Analog Sensor Value: ");
  Serial.println(analogSensorValue);

  if(totalvalues < 3)
  {
    if(ph >= 5.1 && ph <= 7.5)
    {
      k =0;
      if(volt < 5)
      {
        j =0;
        last = last +1;
        if(last == 1)
        {
          bot.sendMessage(CHAT_ID, "pH and turbidity are in range");
        }
        digitalWrite(motor, LOW);
      }
      else
      {
        digitalWrite(motor, HIGH);
        last = 0;
        j = j+1;
        if(j == 1)
        {
          bot.sendMessage(CHAT_ID, "High turbid water");
        }
      }
    }
    else
    {
      digitalWrite(motor, HIGH);
      last = 0;
      k = k+1;
      if( k == 1)
      {
        bot.sendMessage(CHAT_ID, "ph is out of range");
      }
    }
  }
  else
  {
    digitalWrite(motor, HIGH);
    last = 0;
  }
}

void dht_data()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
    
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %, Temp: ");
  Serial.print(t);
  Serial.println(" °Celsius");
}
float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}