//  _      _ _            _     _       ______  _____ _____     _____                  _            
// | |    (_) |          | |   (_)     |  ____|/ ____|  __ \   / ____|                | |           
// | |     _| |_ ___  ___| |__  _ ___  | |__  | (___ | |__) | | |     ___  _   _ _ __ | |_ ___ _ __ 
// | |    | | __/ __|/ __| '_ \| / __| |  __|  \___ \|  ___/  | |    / _ \| | | | '_ \| __/ _ \ '__|
// | |____| | |_\__ \ (__| | | | \__ \ | |____ ____) | |      | |___| (_) | |_| | | | | ||  __/ |   
// |______|_|\__|___/\___|_| |_|_|___/ |______|_____/|_|       \_____\___/ \__,_|_| |_|\__\___|_|   
// Ver 1.0                                                                                                  

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define PIN_SHIFT D1   // verbunden mit SHCP
#define PIN_STORE D2   // verbunden mit STCP
#define PIN_DATA  D3   // verbunden mit DS

// Definiere Wifi Login-Daten
const char *ssid = "ESP8266_Counter";
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

  // Definiere die Pins als Outputs
  pinMode(PIN_STORE, OUTPUT);
  pinMode(PIN_SHIFT, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);

  // Beginne serielles Protokoll
  Serial.begin(9600);

  // Starte Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Starte Web Server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  displayNumber(currentNumber);
}

void loop() {
  // Update die angezeige Nummer
  displayNumber(currentNumber);

  // Schreibe die aktuelle Zahl ins serielle Protokoll
  Serial.print("Number: ");
  Serial.println(currentNumber);

  // Web Server Anfragen annehmen
  server.handleClient();

  // Alle 0.5s die Zahl nochzählen
  delay(500);
  currentNumber = (currentNumber + 1) % 10000;
}

// Funktion zum Anzeigen der Nummer
void displayNumber(int number) {
  int thousands = number / 1000;
  int hundreds = (number % 1000) / 100;
  int tens = (number % 100) / 10;
  int ones = number % 10;

  // Den Schreibschutz-Pin auf Low ziehen um neue Daten zu übermitteln
  digitalWrite(PIN_STORE, LOW);

  // Die Daten für die einzelnen Ziffern rückwärts nacheinander binär ausgeben
  shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[thousands]);
  shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[hundreds]);
  shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[tens]);
  shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[ones]);

  // Den Schreibschutz-Pin auf High setzen um die neuen Daten direkt anzuzeigen
  digitalWrite(PIN_STORE, HIGH);
}

// Web Server Anfragen annehmen
void handleRoot() {
  String html = "<html><head><title>ESP8266 Counter</title></head><body>";
  html += "<h1>Aktueller Zahlenwert: ";
  html += currentNumber;
  html += "</h1></body></html>";
  server.send(200, "text/html", html);
}