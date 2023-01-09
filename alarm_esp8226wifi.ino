#include <IRremote.h>

#define RED D6
#define GREEN D5
#define BUZZER D3
#define LASER D7

IRrecv irrecv(D2);
decode_results results;
unsigned long key_value = 0;

int sensorValue = 0;
int alarm = 0;
int panic = 0;
int i = 15;

void setup(){
  Serial.begin(115200);
  irrecv.enableIRIn();
  irrecv.blink13(true);

  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(LASER, OUTPUT);

  digitalWrite(BUZZER, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(LASER, HIGH);
  delay(1000);
  digitalWrite(BUZZER, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(LASER, LOW);

  delay(2000);
  pinMode(A0, INPUT);

}

int power = 1;

void loop(){

  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(LASER, OUTPUT);

  digitalWrite(LASER, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  Serial.println("Alarm  zapnut");

  alarm = 0;

  while(alarm == 0){

    irrecv.resume(); 

    if (irrecv.decode(&results)){
  
      if (results.value == 0xFFC23D){
        results.value = key_value;

        Serial.println("Potvrdit");

        key_value = results.value;
        irrecv.resume(); 

        i = 15;

        for(i = 15; i > -1; i--){
          digitalWrite(RED, HIGH);
          delay(500);
          digitalWrite(RED, LOW);
          delay(500);
        
        }
        alarm = 1;
      }
    }
  }

  while(alarm == 1){
    digitalWrite(RED, HIGH);
    delay(100);
    sensorValue = analogRead(A0);
    Serial.print("Svetlo: ");
    Serial.println(sensorValue);

    if(sensorValue < 700){
      panic = 1;
      break;
    } 
    //IR odblokování
    if (irrecv.decode(&results)){

      if (results.value == 0xFFC23D){
        results.value = key_value;

        alarm = 0;
        delay(1000);
        key_value = results.value;
        irrecv.resume(); 

      }
    }
    irrecv.resume();
    digitalWrite(LASER, LOW);
    digitalWrite(RED, LOW);
    delay(900);
  }
  while(panic == 1){
    digitalWrite(LASER, LOW);
    digitalWrite(RED, LOW);

    i = 15;
    for(i = 15; i > -1; i--){
      key_value = results.value;
      irrecv.resume();
      digitalWrite(RED, HIGH);

      delay(500);
      digitalWrite(RED, LOW);
      delay(500);

      if (irrecv.decode(&results)){
        if (results.value == 0xFFC23D){
          results.value = key_value;

          alarm = 0;
          panic = 0;
          delay(1000);
          key_value = results.value;
          irrecv.resume();
          break;
        }
      }
    }
    
    while(panic == 1){
      irrecv.resume();
      delay(250);
      digitalWrite(RED, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(250);
      digitalWrite(RED, LOW);
      digitalWrite(BUZZER, LOW);

      if (irrecv.decode(&results)){
        if (results.value == 0xFFC23D){
          results.value = key_value;

          alarm = 0;
          panic = 0;
          delay(1000);
          key_value = results.value;
          irrecv.resume();
          break;
        }
      }
    }
  }
}