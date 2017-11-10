
//=============================================================//
//========================= LED ===============================//
//=============================================================//

const int BLINKING_LED_PIN = 8;
unsigned long previousMillis = 0;
unsigned long blinkInterval = 100;
const int blinkMaxCount = 5;
int blinkCounter = 0;
bool allowLEDBlink = false;

/* Para usar a funcao, basta atribuir a variavel allowLEDBlink para true
    Assim que o metodo de envio/recebimento da informacao de voces acontecer
    O piscamento do LED vai acontecer conforme a formula abaixo e o clock do Arduino
    para que nao bloqueie outras funcoes. Se prejudicar o codigo de voces, 
    me avisem que eu mudo.
 */
bool blinkLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > blinkInterval) {
    if (blinkMaxCount >= blinkCounter) {
      previousMillis = currentMillis;
      swapPhase();
      blinkCounter++;
      return true;
    } else {
      blinkCounter = 0;
      return false;
    }
  }
}

void swapPhase() {
  digitalWrite(BLINKING_LED_PIN, !digitalRead(BLINKING_LED_PIN));
}

void setup() {
  pinMode(BLINKING_LED_PIN, OUTPUT);
}

void loop() {
  // Esse codigo precisa ficar na raiz do loop
  if (allowLEDBlink) {
    // Isso aqui faz com que o proprio metodo volte a variavel para false quando terminar sua tarefa
    allowLEDBlink = blinkLED();
  }
}
