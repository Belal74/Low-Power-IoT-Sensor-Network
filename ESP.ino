#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// --- SETTINGS ---
const char* ssid = "Belal ";
const char* password = "111111112";

// 2. YOUR THINGSPEAK DETAILS
unsigned long myChannelNumber = 3212137; // Replace with your Channel ID
const char * myWriteAPIKey = "GLP9QKU2FXJZ6ZK2"; // Replace with your Write API Key


RF24 radio(D4, D8); 
const byte address[6] = "12345";
WiFiClient client;

// Storage for the latest data received
float latestTemp = 0;
float latestHum = 0;
float latestLDR = 0;

unsigned long lastCloudUpload = 0;
const unsigned long uploadInterval = 20000; // 20 Seconds

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  
  ThingSpeak.begin(client);

  if (!radio.begin()) { Serial.println("nRF24 Error"); while (1); }
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  Serial.println("\n=== STABLE GATEWAY READY ===");
}

void loop() {
  // 1. ALWAYS LISTEN FOR RADIO
  if (radio.available()) {
    char receivedMessage[32] = {0};
    radio.read(receivedMessage, sizeof(receivedMessage));
    String data = String(receivedMessage);
    
    Serial.print("RF In: "); Serial.println(data);

    // Parse Node 1 (Temp/Hum)
    if (data.indexOf("N1") >= 0) {
      int tStart = data.indexOf("T=") + 2;
      int tEnd = data.indexOf("C");
      int hStart = data.indexOf("H=") + 2;
      int hEnd = data.indexOf("%");
      
      latestTemp = data.substring(tStart, tEnd).toFloat();
      latestHum = data.substring(hStart, hEnd).toFloat();
      Serial.printf(" >> Saved N1: T=%.2f H=%.2f\n", latestTemp, latestHum);
    } 
    
    // Parse Node 2 (LDR) - FIXED PARSING
    else if (data.indexOf("N2") >= 0) {
      int ldrStart = data.indexOf("LDR=") + 4; // Looks specifically for LDR=
      latestLDR = data.substring(ldrStart).toFloat();
      Serial.printf(" >> Saved N2: LDR=%.2f\n", latestLDR);
    }
  }

  // 2. UPLOAD TO CLOUD ONLY EVERY 20 SECONDS
  if (millis() - lastCloudUpload > uploadInterval) {
    if (latestTemp != 0 || latestLDR != 0) { // Only upload if we have some data
      Serial.println("--- Syncing to ThingSpeak ---");
      
      ThingSpeak.setField(1, latestTemp);
      ThingSpeak.setField(2, latestLDR);
      
      ThingSpeak.setField(3, latestHum);
      
      int status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      
      if (status == 200) {
        Serial.println("--- Update Successful ---");
      } else {
        Serial.print("--- Update Failed. Error: ");
        Serial.println(status);
      }
      lastCloudUpload = millis();
    }
  }
}
