#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <DHT.h>


char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 0;

String myStatus = "As Normal";

//Initializes DHT sensor Variable
DHT dht(2, DHT11);

void setup() {
  Serial.begin(9600);      // Initialize serial 
  while (!Serial) 
  {
  
  }
  dht.begin();  //Begins DHT Sensor
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
}

void loop() {

  //Initialises a variable to store the temperature from the sensor.
  float temp = dht.readTemperature();

  //Sets the 1st field variable to the value gotten by the sensor. It is converted from a float to a
  number1 = temp;

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, number1);

  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  // change the values
  number1++;
  if(number1 > 99){
    number1 = 0;
  }
  
  delay(20000); // Wait 20 seconds to update the channel again
}
