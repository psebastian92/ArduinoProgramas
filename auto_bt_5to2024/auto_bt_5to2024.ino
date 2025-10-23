int TRIG = 4;
int ECO = 5 ;
int DURACION;
int DISTANCIA;

int MOTORD_CTL1 = 8;
int MOTORD_CTL2 = 9;
int MOTORI_CTL1 = 10;
int MOTORI_CTL2 = 11;
char option = '\0'; // inicializamos con valor nulo las opciones, para evitar conflictos

void setup() {
  Serial.begin(9600);
  pinMode(MOTORD_CTL1, OUTPUT);
  pinMode(MOTORD_CTL2, OUTPUT);
  pinMode(MOTORI_CTL1, OUTPUT);
  pinMode(MOTORI_CTL2, OUTPUT);
  pinMode (TRIG, OUTPUT);
  pinMode (ECO, INPUT);
  if (Serial.available() > 0) {
     Serial.println("Conexion BT exitosa");
  }
}
void loop() {
  if (Serial.available() > 0) {
    char newOption = Serial.read();

    if (option != newOption) { // Comparamos con la opción anterior
      option = newOption; // Actualizamos option con la nueva opción
      Serial.print("Opcion recibida: ");
      Serial.println(option); // Mostramos el mensaje solo si es diferente
    }

    controlManual( frenarAdelantePorUltraSonido()  );

  }
}

bool frenarAdelantePorUltraSonido() {
  digitalWrite(TRIG, LOW);
  delay(1);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);

  DURACION = pulseIn(ECO, HIGH);
  DISTANCIA = DURACION / 58.2;

  delay(3);

  if (DISTANCIA <= 20 && DISTANCIA >= 0) {
    return true;
  }
  else {
    return false;
  }
}

void controlManual(bool frenar) {

  if (frenar == false) {
    if (option == 'S') {
      iStop();
      dStop();
    }

    if (option == 'F') {
      dAdelante();
      iAdelante();
    }

    if (option == 'B') {
      dAtras();
      iAtras();
    }

    if (option == 'L') {
      dAdelante();
      iAtras();
    }

    if (option == 'R') {
      dAtras();
      iAdelante();
    }

    if (option == 'I') {
      dStop();
      iAdelante();
    }

    if (option == 'G') {
      iStop();
      dAdelante();
    }

    if (option == 'H') {
      iStop();
      dAtras();
    }
    if (option == 'J') {
      dStop();
      iAtras();
    }
  } else {
    dStop();
    iStop();

        if (option == 'B') {
          dAtras();
          iAtras();
        }
  }

}

void dStop() { //derecho
  digitalWrite(MOTORD_CTL1, LOW);
  digitalWrite(MOTORD_CTL2, LOW);
}

void iStop() {//izquierdo
  digitalWrite(MOTORI_CTL1, LOW);
  digitalWrite(MOTORI_CTL2, LOW);
}

void dAtras() {
  digitalWrite(MOTORD_CTL1, LOW);
  digitalWrite(MOTORD_CTL2, HIGH);
}

void dAdelante() {
  digitalWrite(MOTORD_CTL1, HIGH);
  digitalWrite(MOTORD_CTL2, LOW);//
}
void iAtras() {
  digitalWrite(MOTORI_CTL1, HIGH);
  digitalWrite(MOTORI_CTL2, LOW);
}
void iAdelante() {
  digitalWrite(MOTORI_CTL1, LOW);
  digitalWrite(MOTORI_CTL2, HIGH);//
}
