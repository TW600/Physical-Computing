#include <WiFi.h>

//Define access point credentials 
const char* ssid     = "EXP1-ssid";
const char* password = "EXP1-password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String httpHeader;

//variables to store the current LED states
String statePin16 = "off";
String statePin17 = "off";

//Output variable to GPIO pins
const int ledPin16 = 16;
const int ledPin17 = 17;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 2000;

void setup() {
 // Setup the serial monitor to report status, with 115200 baud
    Serial.begin(115200);
  // This demonstrates an ESP32 Arudino using WiFi modem using Access Poiny
  // The arduino is creating its own network and allowing devices to connect to it.
    
  pinMode(ledPin16, OUTPUT);      // set the LED pin mode to be an output
  digitalWrite(ledPin16, 0);      // turn LED off by default - to be controlled by web client
  pinMode(ledPin17, OUTPUT);       //set the LED pin mode to be an output
  digitalWrite(ledPin17, 0);      // turn LED off by default - to be controlled by web client

  WiFi.softAP(ssid,password); // Start the software as Access Point
  
  // Print IP address in serial monitor so we know what to connect to and then start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin(); // Starts the web server
}

void loop() { // The loop is continuously called
  WiFiClient client = server.available();   // check if we have an incoming client connection

  if (client) {                             // If a new client connects,
    currentTime = millis();                 // Get current connection time, and start a new time session
    previousTime = currentTime;
    Serial.println("New Client detected.");          // print a message out in the serial port
    String currentLine = "";                // Define a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        httpHeader += c;
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

            // turns the GPIOs on and off
            if (httpHeader.indexOf("GET /16/on") >= 0) {
              statePin16 = "on";
              digitalWrite(ledPin16, HIGH);               // turns the LED on
            } else if (httpHeader.indexOf("GET /16/off") >= 0) {
              statePin16 = "off";
              digitalWrite(ledPin16, LOW);                //turns the LED off
            }
            
            if (httpHeader.indexOf("GET /17/on") >= 0) {
              statePin17 = "on";
              digitalWrite(ledPin17, HIGH);               // turns the LED on
            } else if (httpHeader.indexOf("GET /17/off") >= 0) {
              statePin17 = "off";
              digitalWrite(ledPin17, LOW);                //turns the LED off
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS to style the on/off buttons
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: yellowgreen; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 32px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: gray;}</style></head>");

            client.println("<body><h1>Tom White WiFi Test</h1>");
            client.println("<body><h1>Using Access Point</h1>");
            client.println("<p>Control LED State</p>");

            if (statePin16 == "off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">LED ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">LED OFF</button></a></p>");
            }
            if (statePin17 == "off") {
              client.println("<p><a href=\"/17/on\"><button class=\"button\">LED ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/17/off\"><button class=\"button button2\">LED OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
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
    httpHeader = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
