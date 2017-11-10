void setup() {
  Serial.begin(9600);
  while(!Serial){
  }
  pinMode(7, OUTPUT);
}

void loop() {
digitalWrite(7,HIGH);
}
