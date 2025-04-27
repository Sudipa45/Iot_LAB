#include <WiFi.h>
#define FIREBASE_ENABLE_DEBUG
#include <Firebase_ESP_Client.h>
#define FIREBASE_API_KEY "web api key"
#define DATABASE_URL "realtime database url"

String SSID = "wifi name";
String WIFIPASS = "pass";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200); 
  Serial.println("hello world");

  WiFi.begin(SSID.c_str(), WIFIPASS.c_str());
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected!");

  config.api_key = FIREBASE_API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = "database email";
  auth.user.password = "email pass";

  Serial.println("Initializing Firebase...");
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase.begin done");

  delay(3000); // more time for token generation

  if (Firebase.ready()) {
    Serial.println("Firebase is ready");
    if (Firebase.RTDB.setString(&fbdo, "/led", "on")) {
      Serial.println("Data uploaded");
    } else {
      Serial.println("Data not uploaded");
      Serial.println(fbdo.errorReason());
    }
  } else {
    Serial.println("Firebase not ready yet");
  }
}
void loop()
{

}
