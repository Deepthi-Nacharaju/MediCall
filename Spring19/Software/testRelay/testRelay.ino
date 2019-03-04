#define pin14 7
#define pin1 8
#define pin8 6
#define pin7 5
#define Vin 9
#define LED 3

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, INPUT);
  pinMode(pin8, INPUT);
  pinMode(pin14, INPUT);
  pinMode(pin7, INPUT);
  pinMode(Vin, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("Ready");

  //digitalWrite(Vin, HIGH);
  //delay(1000);
  //digitalWrite(LED, HIGH);
  //delay(1000);
  //digitalWrite(LED, LOW);
  //digitalWrite(Vin, LOW);
}

int status14 = 0;
int status1 = 0;
int status8 = 0;
int status7 = 0;

void loop() {
  // put your main code here, to run repeatedly:
  //while (digitalRead(Vin)){
//  if (digitalRead(pin14)==digitalRead(pin1)){
//    Serial.println("pin 14 and 1 in same voltage");
//    delay(5000);
//  }
  //}
  digitalWrite(Vin, LOW); 
  double *command = checkstatus();
    for(int i = 0; i < 3; i++)  {
    Serial.println(command[i]);
    }
  delay(5000);

  digitalWrite(Vin, HIGH);
  double *highcommand = checkstatus();
    for(int i = 0; i < 3; i++)  {
    Serial.println(highcommand[i]);
    }
  delay(5000);
}

double* checkstatus(){
  status14 = digitalRead(pin14);
  status1 = digitalRead(pin1);
  status8 = digitalRead(pin8);
  status7 = digitalRead(pin7);
  
  static double inputs[3];
  //int i = 1;
  while(1){
    if (status14 == status1){
      inputs[0] = 1;
    }
    else if (status14 != status1){
      inputs[0] = 0;
    }
    if (status14 == status7){
      inputs[1] = 1;
    }
    else if (status14 != status7){
      inputs[1] = 0;
    }
    if (status14 == status8){
      inputs[2] = 1;
    }
    else if (status14 != status8){
      inputs[2] = 0;
    }
    
    return inputs;
//    else {
//      //int i = 1;
//      inputs[0] = 0;
//      inputs[1] = 0;
//      inputs[2] = 0;
//      return inputs;
//    }
  }
}

