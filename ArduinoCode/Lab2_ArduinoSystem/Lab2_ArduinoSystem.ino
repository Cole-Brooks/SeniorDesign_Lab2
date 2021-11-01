#include <IRremote.h>

IRrecv irrecv(2);
decode_results res;
int ain = 14;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(ain, INPUT);
  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(irrecv.decode()){
    Serial.println(res.value, HEX);
    irrecv.resume();
  }
  int aV = analogRead(ain);
  if(aV < 800){
    Serial.println(aV);
  }
//  delay(1000);
}
