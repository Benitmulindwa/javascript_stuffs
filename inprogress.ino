#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <Hash.h>


#define SOUND_SPEED 0.034
#define BUZZER 2

// Ultrasonic sensor pins
const int trigPin = 12; // Define Trig pin
const int echoPin = 14; // Define Echo pin
int data;

String old_value, value;

ESP8266WiFiMulti    WiFiMulti;
ESP8266WebServer    server(80);
WebSocketsServer    webSocket = WebSocketsServer(81);

char html_template[] PROGMEM = R"=====(
<html lang="en">
   <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>Canne Intelligente</title>
      <script>
        socket = new WebSocket("ws:/" + "/" + location.host + ":81");
        socket.onopen = function(e) {  console.log("[socket] socket.onopen "); };
        socket.onerror = function(e) {  console.log("[socket] socket.onerror "); };
        socket.onmessage = function(e) {  
            console.log("[socket] " + e.data);
            document.getElementById("mrdiy_value").innerHTML = e.data + " Cm";
            received_data=e.data;
            
        };

        let i=0;
        let distance = parseInt(received_data);

        if(distance<=10){
          speak("Obstacle inferieur à dix centimetres");
        }

        function find_canne(){

          if(i%2==0){
            socket.send(0);
          }
          else{
            socket.send(1);
          }
          i=i+1;
        }

        function speak(my_text){

            if ('speechSynthesis' in window) {
            
            var utterance = new SpeechSynthesisUtterance();

            utterance.lang="fr-FR";
            utterance.text = my_text;
            speechSynthesis.speak(utterance);

          } else {
              console.error("Speech synthesis not supported in this browser.");
          }
        }

        function say_data(){
          speak(received_data + "centimetres");
          
        }
      </script>
   </head>
   <body style="max-width:400px;margin: auto;font-family:Arial, Helvetica, sans-serif;text-align:center">
      <div><h2><br />Canne Intelligente</h2></div>
      <div>
        <button onclick="find_canne()" style="background-color:#cfd0cf; color: black; cursor: pointer; font-size: 20px; font-weight:bold; border-radius: 10px; height: 150px; width:100%; margin: 15px 32px;">Cherche Canne</button>
      </div>
      <div><p id="mrdiy_value" style="font-size:60px;margin:0"></p></div>
      <div>
        <button onclick="say_data()" style="background-color:#cfd0cf; color: black; cursor: pointer; font-size: 20px; font-weight:bold; border-radius: 10px ; height: 150px; width:100%; margin: 15px 32px;">Distance de la canne</button>
      </div>
   </body>
</html>
)=====";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        webSocket.sendTXT(num, "0");
      }
      break;

    case WStype_TEXT:
      //Serial.printf("[%u] get Text: %s\n", num, payload);
      data = atoi((char*)payload);
      digitalWrite(BUZZER, data);
      //Serial.printf("%s\n",data);
      // send message to client
      // webSocket.sendTXT(num, "message here");
      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
      
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
  }

}

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, 1);

  Serial.begin(115200);
  delay(1000);

  WiFiMulti.addAP("Networkname", "password");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.on("/", handleMain);
  server.onNotFound(handleNotFound);
  server.begin();

}

void handleMain() {
  server.send_P(200, "text/html", html_template ); 
}
void handleNotFound() {
  server.send(404,   "text/html", "<html><body><p>404 Error</p></body></html>" );
}

void loop() {
 
//--------------------------------------------------------------------------------------------
  // ULTRASONIC:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  int duration = pulseIn(echoPin, HIGH);
  int distanceCM = duration*SOUND_SPEED/2;
//---------------------------------------------------------------------------------------------
  //COMMUNICATION SERVER->CLIENT
  webSocket.loop();
  server.handleClient();

  value = (String) analogRead(0);
  //Serial.println(value);
  webSocket.broadcastTXT("13");
  delay(50);
}
