char t;

void setup() {
  pinMode(13, OUTPUT);   // Motores izquierdos hacia adelante
  pinMode(12, OUTPUT);   // Motores izquierdos hacia atrás
  pinMode(11, OUTPUT);   // Motores derechos hacia adelante
  pinMode(10, OUTPUT);   // Motores derechos hacia atrás
  pinMode(9, OUTPUT);    // LED
  Serial.begin(9600);    // Inicia la comunicación serial a 9600 baudios
}

void loop() {
  if (Serial.available()) {
    t = Serial.read();        // Lee el carácter recibido por serial
    Serial.println(t);        // Imprime el carácter recibido en el monitor serial
  }

  if (t == 'F') {             // Avanzar (todos los motores giran hacia adelante)
    digitalWrite(13, HIGH);
    digitalWrite(11, HIGH);
  }

  else if (t == 'B') {        // Retroceder (todos los motores giran hacia atrás)
    digitalWrite(12, HIGH);
    digitalWrite(10, HIGH);
  }

  else if (t == 'L') {        // Girar a la derecha (solo giran los motores izquierdos hacia adelante)
    digitalWrite(11, HIGH);
  }

  else if (t == 'R') {        // Girar a la izquierda (solo giran los motores derechos hacia adelante)
    digitalWrite(13, HIGH);
  }

  else if (t == 'W') {        // Encender el LED
    digitalWrite(9, HIGH);
  }

  else if (t == 'w') {        // Apagar el LED
    digitalWrite(9, LOW);
  }

  else if (t == 'S') {        // Detener (todos los motores se apagan)
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
  }

  delay(100); // Espera 100 milisegundos
}
