

#include <WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

const char* ssid = "";  
const char* password = "";   

WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "ZLQBHRXO4P9IU82A";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;


int pino_d = 12; /* Pino D12 do ESP32 ligado ao D0 do sensor */
int pino_a = 35; /* Pino D35 do ESP32 ligado ao A0 do sensor */
int val_d = 0;   /* Armazena leitura do pino digital */
int val_a = 0;   /* Armazena leitura do pino analógico */
float sensorValue;
float chuva;
float temperature;
float pressao;
float altitude;
float humidade;
float sensorVoltage; 
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;

void initBME(){
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void setup() {
  Serial.begin(115200); 
  initBME();
  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  
}

void loop() {


  if ((millis() - lastTime) > timerDelay) {
     

    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(10000);     
      } 
      Serial.println("\nConnected.");
    }


    
      
      
      
      sensorVoltage = (sensorValue/4095)*5;
      Serial.print("sensor reading = ");
      Serial.print(sensorValue);
      Serial.println("");
      Serial.print("sensor voltage = ");
      Serial.print(sensorVoltage);
      Serial.println(" V");
      Serial.println();

      /* Armazena os valores de leitura */
      val_a = analogRead(pino_a);
      /* Se a leitura analógica for menor que 300 */
      if ( val_a < 1000)
      { /* Chuva intensa */
        chuva=1;
        Serial.println("Chuva Intensa");
        Serial.println(val_a );
      }
      /* Se a leitura analógica for menor que 500 e maior que 300 */
      if (val_a <= 3000 && val_a >= 1000)
      { /* Chuva moderada */
        chuva=1;
        Serial.println("Chuva Moderada ou Chuvisco");
        Serial.println(val_a );
      }
      /* Se a leitura analógica for maior que 500 */
      if ( val_a > 4000)
      { /* Sem previsão de Chuva */
      
        Serial.println("Sem previsão de chuva");
        Serial.println(val_a );
        chuva=0;
      }
      Serial.println();

      Serial.print("Temperature = ");
      temperature=bme.readTemperature();
      Serial.print(bme.readTemperature());
      Serial.println(" *C");
      
      // Convert temperature to Fahrenheit
      /*Serial.print("Temperature = ");
      Serial.print(1.8 * bme.readTemperature() + 32);
      Serial.println(" *F");*/
      
      Serial.print("Pressure = ");
      pressao=bme.readPressure() / 100.0F;
      Serial.print(bme.readPressure() / 100.0F);
      Serial.println(" hPa");

      Serial.print("Approx. Altitude = ");
      altitude=bme.readAltitude(SEALEVELPRESSURE_HPA);
      Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
      Serial.println(" m");

      Serial.print("Humidity = ");
      humidade=bme.readHumidity();
      Serial.print(bme.readHumidity());
      Serial.println(" %");

      Serial.println();


      ThingSpeak.setField(1, temperature);
    //ThingSpeak.setField(1, temperatureF);
      ThingSpeak.setField(2, pressao);
      ThingSpeak.setField(3, altitude);
      ThingSpeak.setField(4, humidade);
      ThingSpeak.setField(5, sensorVoltage);
      ThingSpeak.setField(6, chuva);

    if(true){ 
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if(x == 200){
        Serial.println("Channel update successful.");
      }
      else{
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
      lastTime = millis();
    }
  else{
    Serial.println("nao foi atualizado");
    lastTime = millis();
  }
}}





