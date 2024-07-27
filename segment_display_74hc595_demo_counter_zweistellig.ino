#define PIN_SHIFT D1   // connected to SHCP
#define PIN_STORE D2   // connected to STCP
#define PIN_DATA  D3   // connected to DS

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

void setup() {
  pinMode(PIN_STORE, OUTPUT);
  pinMode(PIN_SHIFT, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 100; i++) {
    int tens = i / 10;
    int ones = i % 10;

    digitalWrite(PIN_STORE, LOW);

    // Send the tens digit to the second display
    shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[tens]);
    // Send the ones digit to the first display
    shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[ones]);

    digitalWrite(PIN_STORE, HIGH);

    Serial.print("Number: ");
    Serial.println(i);

    delay(500);
  }
}