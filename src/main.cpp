#include <Arduino.h>

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

const int output = 2;
const int buttonPin = 4;

//clean-up
int analogPin = 3;
int cx=0;

const char* ssid = "p-v";
const char* password = "3.1415926";
WebServer server(80);

String outputState()
{
  if (digitalRead(output)){return "checked";}
  else {return "";}
  return "";
}

String processor(const String& var)
{
  if (var == "BUTTONPLACEHOLDER")
  {
    String buttons="";
    String outputStateValue = outputState();
    buttons += "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider round\"></span></label>";
    return buttons;
  } 
}


void handleRoot() {
  Serial.println(server.args());

  for (uint8_t i = 0; i < server.args(); i++) {
   if (server.argName(i) == "Sekcja-1") butt1state = (server.arg(i) == "on");
   if (server.argName(i) == "Sekcja-2") butt2state = (server.arg(i) == "on");
  }

  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);

  // server.send(200, "text/html", "");
  const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
<html>
<head>
  <title>Szklarnia</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
      font-family: Arial, sans-serif;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
      background-color: #f0f0f0;
    }
    body {
      max-width: 600px;
      margin: 0 auto;
      padding: 20px;
      background: white;
      border-radius: 8px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }
    h1 {
      font-size: 2rem;
      color: #333;
    }
    p {
      font-size: 1.5rem;
      color: #555;
    }
    .switch {
      position: relative;
      display: inline-block;
      width: 100px;
      height: 50px;
      margin: 10px;
    }
    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }
    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      border-radius: 50px;
      transition: .4s;
    }
    .slider:before {
      position: absolute;
      content: "";
      height: 40px;
      width: 40px;
      border-radius: 50%;
      left: 5px;
      bottom: 5px;
      background-color: #fff;
      transition: .4s;
    }
    input:checked + .slider {
      background-color: #4CAF50;
    }
    input:checked + .slider:before {
      transform: translateX(50px);
    }
    .label-container {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin: 10px 0;
    }
    .label-container span {
      font-size: 1.2rem;
      color: #333;
    }
  </style>
</head>
<body>
  <h1>Szklarnia</h1>
  <p>Ustawienia Przekaznikow:</p>

  <div class="label-container">
    <span>Sekcja Pomidory</span>
    <label class="switch">
      <input type="checkbox" id="button1" onchange="updateState('button1', this.checked)">
      <span class="slider"></span>
    </label>
  </div>

  <div class="label-container">
    <span>Sekcja Alternatywna</span>
    <label class="switch">
      <input type="checkbox" id="button2" onchange="updateState('button2', this.checked)">
      <span class="slider"></span>
    </label>
  </div>

  <iframe name="ifr" style="display:none"></iframe>

  <script>
    function updateState(buttonId, state) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/update?button=" + buttonId + "&state=" + (state ? 1 : 0), true);
      xhr.send();
    }

    document.addEventListener('DOMContentLoaded', function () {
    });
  </script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", index_html);
  /* Temporary solution 
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
  */
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
  digitalWrite(RELAY1_PIN, butt1state);
  server.send(200, "text/plain", "");
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

  server.on("/", handleRoot);
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