void setup() {
  Serial.begin(9600);
  while(!Serial){
  }
  pinMode(6, OUTPUT);
}

void loop() {
digitalWrite(6,HIGH);
}
