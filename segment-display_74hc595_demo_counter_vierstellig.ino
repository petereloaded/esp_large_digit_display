#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define PIN_SHIFT D1   // connected to SHCP
#define PIN_STORE D2   // connected to STCP
#define PIN_DATA  D3   // connected to DS

const char *ssid = "ESP8266_Hotspot";
const char *password = "12345678";  // Mindestens 8 Zeichen

ESP8266WebServer server(80);

const byte numbers[10] = {
  ~0b11111110, // 0
  ~0b00110000, // 1
  ~0b10101101, // 2
  ~0b10111001, // 3
  ~0b00110011, // 4
  ~0b10011011, // 5
  ~0b10011111, // 6
  ~0b10110000, // 7
  ~0b10111111, // 8
  ~0b10111011  // 9
};

int currentNumber = 0;

void setup() {
  pinMode(PIN_STORE, OUTPUT);
  pinMode(PIN_SHIFT, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);

  Serial.begin(9600);

  // Start Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Start Web Server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  displayNumber(currentNumber);
}

void loop() {
  // Update the number displayed
  displayNumber(currentNumber);

  // Print the number to Serial Monitor
  Serial.print("Number: ");
  Serial.println(currentNumber);

  // Handle web server requests
  server.handleClient();

  // Increment the number every 500ms
  delay(500);
  currentNumber = (currentNumber + 1) % 10000;
}

void displayNumber(int number) {
  int thousands = number / 1000;
  int hundreds = (number % 1000) / 100;
  int tens = (number % 100) / 10;
  int ones = number % 10;

  digitalWrite(PIN_STORE, LOW);

  // Send the digits to the displays in reverse order
  shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[thousands]);
  shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[hundreds]);
  shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[tens]);
  shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[ones]);

  digitalWrite(PIN_STORE, HIGH);
}

void handleRoot() {
  String html = "<html><head><title>ESP8266 Counter</title></head><body>";
  html += "<h1>Current Number: ";
  html += currentNumber;
  html += "</h1></body></html>";
  server.send(200, "text/html", html);
}