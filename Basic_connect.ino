#include <WiFi.h>              
#include <SoftwareSerial.h>
#include <FirebaseESP32.h>
#include <TinyGPS++.h>         

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "POWER_HOUSE 1917"
#define WIFI_PASSWORD "power_housee"
#define USER_EMAIL "tinomafaune@gmail.com"
#define USER_PASSWORD "**"

// Insert Firebase project API Key
#define API_KEY "**"
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://live-stock-tracker-ab5a7-default-rtdb.asia-southeast1.firebasedatabase.app/" 

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int count = 0;

TinyGPSPlus gps;
FirebaseData firebaseData;
SoftwareSerial ss(17, 12); // RX, TX pins for GPS module (SoftwareSerial is used for ESP32)

struct GPSData {
  float latitude;
  float longitude;
};
GPSData gpsData;

unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 2000;

void setup(){

  ss.begin(9600);
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  config.max_token_generation_retry = 5;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}

void loop(){

    while (ss.available() > 0) {
      gps.encode(ss.read());
        if ( gps.location.isUpdated()) {
          Serial.print("Latitude : ");
          gpsData.latitude=gps.location.lat();
          Serial.println(gpsData.latitude, 6);
          Serial.print("Longitude : ");
          gpsData.longitude=gps.location.lng();
          Serial.println(gpsData.longitude, 6);
        }
    }
  

  /*if (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    while(true){
      Firebase.setDouble(fbdo, "Coordinates/Latitude", gpsData.latitude);
      Firebase.setDouble(fbdo, "Coordinates/Longitude", gpsData.longitude);
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
   
}*/

}