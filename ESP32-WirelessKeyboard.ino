// Load Wi-Fi library
#include <WiFi.h>
#include <BleKeyboard.h>

// Replace with your network credentials
const char* ssid     = "GT1308";
const char* password = "13081308";

BleKeyboard bleKeyboard("FinalKey V65", "FinalKey");

// Set web server port number to 80
WiFiServer server(80);

// Decode HTTP GET value
String message = "0";
int pos1 = 0;
int pos2 = 0;

// Variable to store the HTTP req  uest
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

IPAddress local_IP(192, 168, 100, 9);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); 
IPAddress secondaryDNS(8, 8, 4, 4);

void setup() {
  Serial.begin(115200);
  bleKeyboard.begin();

if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
Serial.println("STA Failed to configure");
}
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  //server.handleClient();
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Request sample: /?r201g32b255&
            // Red = 201 | Green = 32 | Blue = 255
            if(header.indexOf("GET /?&") >= 0) {
              pos1 = header.indexOf('&');
              pos2 = header.indexOf('@');
              
              message = header.substring(pos1+1, pos2);
              Serial.println(message);
              
              bleKeyboard.print(message + " ");
              
              /*Serial.println(message.toInt());
              Serial.println(greenString1.toInt());
              Serial.println(blueString1.toInt());*/
            }
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
