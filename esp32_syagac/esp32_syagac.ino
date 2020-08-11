
// Import required libraries
#ifdef ESP32
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#else
#include <Arduino.h>
#include <interrupt.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif
//interrupt related
const byte interruptPin = 18;
volatile int interruptCounter = 0;
volatile int numberOfInterrupts = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// Replace with your network credentials
const char* ssid = "AndroidAP";
const char* password = "ktwn1717";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

int count = 0;
int count_pulse;

String SaygacdanDeyeriOxu() {
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&mux);
    interruptCounter--;
    portEXIT_CRITICAL(&mux);
    numberOfInterrupts++;
    Serial.print("Ala: ");
    Serial.println(numberOfInterrupts);
  }
  float DeyerS = numberOfInterrupts ; //saygacdan oxunan deyer
  DeyerS = DeyerS /100;
  return String(DeyerS);
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2> SMART QAZ SAY&#286ACI </h2>
  

  
  <p>
    <i class="fas fa-fire" style="color:#F4490E;"></i> 
    <span class="ds-labels">S&#601rfiyyat:</span>
    <span id="temperaturef">%TEMPERATUREF%</span>
    
    <sup class="units">m&#179</sup>
    
  </p>
  
</body>
<script>


setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();
}, 100) ; // 10000
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var) {
  //Serial.println(var);

  if (var == "TEMPERATUREF") {
    return SaygacdanDeyeriOxu();
  }
  return String();
}
void IRAM_ATTR isr() {
  portENTER_CRITICAL_ISR(&mux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&mux);
}
void setup() {
  pinMode(interruptPin, INPUT);
 attachInterrupt(interruptPin, isr, RISING);
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });


  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", SaygacdanDeyeriOxu().c_str());
  });
  // Start server
  server.begin();
}

void loop() {

}
