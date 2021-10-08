void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!");
  pinMode(2,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int laser = digitalRead(2);
  if(laser == 1){
    Serial.println("laser detected");
  }
  else{
    Serial.println("No laser");
  }
  delay(100);
}
