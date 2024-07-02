#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Default delay time in milliseconds
unsigned long delayTime = 30000;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  // Start the DS18B20 sensor
  sensors.begin();

  // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  bool res;
  res = wm.autoConnect("AutoConnectAP", "password");  // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
}

void loop() {
  // Get temperature readings
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");

  if ((WiFi.status() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation
    client->setInsecure();

    //create an HTTPClient instance
    HTTPClient https;

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    String serverUrl = "https://script.google.com/macros/s/remplaceWithYourURL/exec?sensor=sensor1&temp=" + String(temperatureC);
    
    while (true) {
      if (https.begin(*client, serverUrl)) {  // HTTPS
        Serial.print("[HTTPS] GET...\n");


        const char* headerNames[] = { "Location" };
        https.collectHeaders(headerNames, sizeof(headerNames)/sizeof(headerNames[0]));

        // start connection and send HTTP header
        int httpCode = https.GET();
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);

            // Convert payload to integer and update delayTime
            int newDelayTime = payload.toInt();
            if (newDelayTime > 0) {
              delayTime = newDelayTime * 1000; // Convert seconds to milliseconds
            }
            break;
          } else if (httpCode == HTTP_CODE_FOUND) {  // 302 redirect

            String newLocation = https.header("Location");

            //String newLocation = https.header("Location");
            Serial.print("[HTTPS] Redirect to: ");
            Serial.println(newLocation);
            serverUrl = newLocation;  // Update URL for the redirect
            https.end();
            continue;  // Repeat the GET request with the new URL
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          break;
        }

        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
        break;
      }
    }
  }

  delay(delayTime);
}
