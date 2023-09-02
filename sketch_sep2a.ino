#include <SPI.h>
#include <WiFiNINA.h>

//  WiFi Credentials
char wifiSsid[] = "OnePlus Nord CE 2 Lite 5G"; // WiFi SSID
char wifiPassword[] = "12345678"; // WiFi Password

// IFTTT webhook configuration
const char* iftttWebhookKey = "lDD3BXpJCZd-lcDfWFg2s5Au4Gri5VFGht6R_6NCjnG"; // IFTTT Webhook key
const char* sunlightStartEvent = "Sunlight_Is_Detected"; // event name for sunlight detection
const char* sunlightStopEvent = "Sunlight_Has_Stopped"; // event name when no sunlight

const int ldrPin = A0; // Analog pin for LDR
bool isSunlightDetected = false; // Flag to check if sunlight is present or not

WiFiClient wifiClient; 

void setup() {
  Serial.begin(9600); 

  // Connecting to WiFi network
  while (WiFi.begin(wifiSsid, wifiPassword) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting Up WiFi Connection");
  }
  Serial.println("Wi-Fi connection established");
}

void loop() {
  int ldrValue = analogRead(ldrPin); // Reading the LDR sensor value

  // Conditional statement to check if sunlight is there and then a notification is sent
  if (!isSunlightDetected && ldrValue < 600) {
    sendIFTTTEvent(sunlightStartEvent);
    isSunlightDetected = true;
  } 
 
  else if (isSunlightDetected && ldrValue > 600) {
    sendIFTTTEvent(sunlightStopEvent);
    isSunlightDetected = false;
  }

  delay(1500); 
}

// Procedure to send an event to IFTTT using Webhooks
void sendIFTTTEvent(const char* eventName) {
  if (wifiClient.connect("maker.ifttt.com", 80)) {
    Serial.println("Connected to IFTTT");

    String url = "/trigger/";
    url += eventName;
    url += "/with/key/";
    url += iftttWebhookKey;

    wifiClient.println("POST " + url + " HTTP/1.1");
    wifiClient.println("Host: maker.ifttt.com");
    wifiClient.println("Connection: close\r\n\r\n");
    delay(10);

    while (wifiClient.available()) {
      char c = wifiClient.read();
      Serial.print(c); // Printing the response from IFTTT
    }
    Serial.println();
    wifiClient.stop();
    Serial.println("IFTTT email sent");
  } else {
    Serial.println("Unable to send the email");
  }
}
