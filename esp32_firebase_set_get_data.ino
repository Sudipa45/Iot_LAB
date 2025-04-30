#include <WiFi.h>
#define FIREBASE_ENABLE_DEBUG
#include <Firebase_ESP_Client.h>
#define FIREBASE_API_KEY "web_api_key"
#define DATABASE_URL "database_url"

String SSID = "ssid";
String WIFIPASS = "pass";
String FDB = "",prev_fdb ="";

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
  auth.user.email = "database_gmail";
  auth.user.password = "gmail_pass";

  Serial.println("Initializing Firebase...");
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase.begin done");

  delay(3000); // more time for token generation

  if (Firebase.ready()) {
    Serial.println("Firebase is ready");
    if (Firebase.RTDB.setString(&fbdo, "/led/state", "hello from esp")) {
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
Cloudtask();
  delay(500);
}
void Cloudtask() {
  if (Firebase.ready()) {
    if (Firebase.RTDB.getString(&fbdo, "/led/state")) {
      FDB = fbdo.stringData();
      FDB.trim();
      if (prev_fdb != FDB) {
        Serial.println(FDB);
        data_engine(FDB);
        prev_fdb = FDB;
      }
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
}

void data_engine(String DATA) {
  int pre_index = 0;
  for (int i = 0; i < DATA.length(); i++) {
    if (DATA[i] == '~') {
      Serial.println(DATA.substring(pre_index, i));
      pre_index = i + 1;
    }
  }
  // print the last command if no trailing '~'
  if (pre_index < DATA.length()) {
    Serial.println(DATA.substring(pre_index));
  }
}