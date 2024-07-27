#define PIN_SHIFT D1   // connected to SHCP
#define PIN_STORE D2   // connected to STCP
#define PIN_DATA  D3  // connected to DS

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

void setup()
{
  pinMode(PIN_STORE, OUTPUT);
  pinMode(PIN_SHIFT, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);

  Serial.begin(9600);

}

void loop ()
{
 for(byte i=0; i<sizeof(numbers); i++) {
    digitalWrite(PIN_STORE, LOW);
    shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, numbers[i]);
    digitalWrite(PIN_STORE, HIGH);
    Serial.println(i);
    delay(500);
  }
}