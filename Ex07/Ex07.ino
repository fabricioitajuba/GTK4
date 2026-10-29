int contador = 0;

void setup() {
  Serial.begin(9600); // Mesma velocidade configurada no GTK
}

void loop() {
  Serial.println(contador); // Envia o número e pula a linha
  contador++;
  delay(1000); // Incrementa a cada 1 segundo
}
