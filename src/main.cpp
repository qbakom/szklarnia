#include <Arduino.h>
#include <FastLED.h>

#include <stdbool.h>
#include "soc/soc.h"
#include "soc/gpio_periph.h"
#include "soc/gpio_struct.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include "WiFi.h"
#include <Wire.h>
#include <Button2.h>
#include "esp_adc_cal.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "SPI.h"
#include "TFT_eSPI.h"


#include "Button2.h"
#ifndef TFT_DISPOFF
#endif

#ifndef TFT_SLPIN
#endif


#define RELAY1_PIN       32
#define RELAY2_PIN       26
//clean-up
#define ADC_EN          14
#define ADC_PIN         34
#define BUTTON_1        35
#define BUTTON_2        0


using std::string;

bool butt1state = false;
bool butt2state = false;

//clean-up
int ledPin = 27;
int analogPin = 3;
int cx=0;
int leds_num = 40;
CRGB leds[36];


int pom = 0;
int bri = 10;
int wsk = 0;
int count = 0;
int gr = 0;
int red = 1;
int blue = 0;


const char* ssid = "p-v";
const char* password = "3.1415926";
WebServer server(80);

int counter = 0;

int period = 0;

void handleRoot() {
  counter = -100;
  Serial.println(server.args());

  for (uint8_t i = 0; i < server.args(); i++) {
   if (server.argName(i) == "Sekcja-1") butt1state = (server.arg(i) == "on");
   if (server.argName(i) == "Sekcja-2") butt2state = (server.arg(i) == "on");
  }

  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);

  server.send(200, "text/html", "<html>"
  "<head><style>html { font-family: Times New Roman,Arial,sans-serif; display: inline-block; margin: 0px auto; text-align: center;}"
  "text-decoration: none; font-size: 30px; margin: 2px;}"
  "</style></head>"
  "<body>"
  "<p>RELAY settings:</p>"
  "<input type=checkbox name=button1 onchange=\"this.form.submit();\" " + String(butt1state ? "checked" : "") + "><label for=button1>Sekcja 1</label></input>"
  "<input type=checkbox name=button2 onchange=\"this.form.submit();\" " + String(butt2state ? "checked" : "") + "><label for=button2>Sekcja 2</label></input>"
  "<iframe name=ifr style=\"display:none\"></iframe>"
  "</body></html>");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  } 
  server.send(404, "text/plain", message);
}

void handlebutton1()
{
  butt1state = !butt1state;
  digitalwrite(RELAY1_PIN, butt1state);
}

void setup() 
{
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  // server.on("/", handleRoot);
  server.on("/pomidory", handleRoot);
  server.on("/button2", handleRoot);



  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}


void loop() 
{   
    server.handleClient();
    
    delay(150);

}