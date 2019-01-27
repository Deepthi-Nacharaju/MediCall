int LED1 = 3;
int LED2 = 4;
int LED3 = 5;
int LED4 = 6;
int LED5 = 7;
int signalPin = 2;
int counter = 1;
unsigned long timer = 0;
bool signal_switch = true;
unsigned long signal_lost = 0;
unsigned long read_pot = 0;
float call_delay_float = 0.0000;
int call_delay = 0;
#define pot A5
// Pot step 0.01265 Seconds
void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(signalPin, INPUT);
  pinMode(pot, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(signalPin);
  read_pot = analogRead(pot);
  call_delay_float = read_pot * 0.0127;
  call_delay = call_delay_float * 1000;
  //Serial.println(call_delay);
  while (!value) {
    if (counter) {
      timer = millis();
      counter = 0;
      //Serial.println(timer);
      //Serial.println(counter);
    }
    if (millis() - timer > call_delay) {
      blink_lights();
      blink_lights();
      signal_lost = 0;
      signal_switch = true;
      Serial.println("Call Placed");
      break;
    }
    if (millis() - timer > call_delay / 5 * 4) {
      digitalWrite(LED5, HIGH);
    }
    else if (millis() - timer > call_delay / 5 * 3) {
      digitalWrite(LED4, HIGH);
    }
    else if (millis() - timer > call_delay / 5 * 2) {
      digitalWrite(LED3, HIGH);
    }
    else if (millis() - timer > call_delay / 5 * 1) {
      digitalWrite(LED2, HIGH);
    }
    else if (millis() - timer > 10) {
      digitalWrite(LED1, HIGH);
    }
    int value = digitalRead(signalPin);
    if (value && signal_switch == true){
      signal_lost = millis();
      signal_switch = false;
    }
    if (!value){
      signal_lost = 0;
      signal_switch = true;
    }
    if (millis() - signal_lost > 500 && signal_switch == false){
      break;
    }
    //Serial.println(value);
  }
  counter = 1;
  reset_lights();
}

void reset_lights() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
}

void lights_on(){
  digitalWrite(LED5, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);
}
void blink_lights(){
  lights_on();
  delay(250);
  reset_lights();
  delay(250);
  lights_on();
  delay(250);
  reset_lights();
  delay(250);
}

