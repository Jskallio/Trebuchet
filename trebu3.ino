
#include <Servo.h>

Servo lockServo;
Servo motorServo;
//Pins
const int trigPin = 12;
const int echoPin = 11;
//const int motorPin = 10;
const int lockPin = 9;
//const int motorRunPin = 8;

//States
//const int KELAUS = 0;
const int LOCK = 1;
const int ODOTUS = 2;
const int LAUKAISU = 3;

//ServoPositions
const int lockOn = 0;
const int lockOff = 30;
//const int motorOn = 120;
//const int motorOff = 180;

//WaitTimes (milliseconds)
//const int kelausAika = 7500;
const int lukitusAika = 500;
const int laukaisuAika = 5000;

//ultraääni raja
const int raja = 5;


long duration;
int distance;
unsigned long tapahtumaAika;
int dis2;
int olisama;

bool vaihto = true;
int state = 1;
void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //pinMode(motorRunPin, OUTPUT);
  lockServo.attach(lockPin);
  //motorServo.attach(motorPin);
  Serial.begin(9600);
}

void loop() {
  switch(state){
    case LOCK:
      if(vaihto == true){
        tapahtumaAika = millis();
        vaihto = false;
        lockServo.write(lockOn);
      }

      if(millis()>tapahtumaAika+lukitusAika){
        state = ODOTUS;
        vaihto = true;
        Serial.println("Vaihe: odotus");
      }
      break;
    case ODOTUS:
      delay(10);
      if(vaihto == true){
        olisama = 0;
        vaihto = false;
      }
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      // Calculating the distance
      dis2= duration*0.034/2;
      
      if(dis2 < raja && dis2>0){
        olisama = olisama+1;
      }

      if(olisama > 5){
        state = LAUKAISU;
        vaihto = true;
        Serial.println(dis2);
        Serial.println("Vaihe: Laukaisu");
      }
      


      
      break;
    case LAUKAISU:
      if(vaihto == true){
        lockServo.write(lockOff);
        tapahtumaAika = millis();
        vaihto = false;
        Serial.println(tapahtumaAika);
        Serial.println(millis());
      }

      if(millis()>tapahtumaAika+laukaisuAika){
        Serial.println(millis());
        state = LOCK;
        vaihto = true;
        tapahtumaAika = millis();
        Serial.println("Vaihe: Lukitus");
      }
      break;
  }
}
