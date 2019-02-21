#define IRpin_PIN PIND
#define IRpin     2
#define MAXPULSE 65000
#define RESOLUTION 20
int LED1 = 3;
int LED2 = 4;
int LED3 = 5;
int LED4 = 6;
int LED5 = 7;
int signalPin = 2

uint16_t pulses[100][2];
uint8_t currentpulse = 0;
long interval = 1000;
unsigned long previous_detect = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Ready to detect!");
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(signalPin, INPUT);
}

void loop(void) {
  // put your main code here, to run repeatedly:
  uint16_t highpulse, lowpulse;
  highpulse = lowpulse = 0;

  while (IRpin_PIN & (1<<IRpin)) {
    // Serial.println("OFF");
    // digitalWrite(LED, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }

  while (! (IRpin_PIN & _BV(IRpin))) {
    unsigned long start_detect = millis();
    if (start_detect - previous_detect > interval);
      digitalWrite(LED1, HIGH);
      unsigned long previous_detect = start_detect;
    if (start_detect - previous_detect > 2*interval);
      digitalWrite(LED2, HIGH);
      previous_detect = start_detect;
    // Serial.println("ON");
    // digitalWrite(LED, HIGH);
  }
}
