#include <Arduino.h>
#include <Servo.h>

#define BASE_PIN 3
#define OMBRO_PIN 5
#define CTVL_PIN 6
#define GARRA_PIN 9

const unsigned int P0[3] = {105, 85, 115};
const unsigned int P1[3] = {105, 110, 115};
const unsigned int P2[3] = {105, 150, 40};
const unsigned int P3[3] = {1, 150, 115};
const unsigned int P4[3] = {1, 150, 40};
const unsigned int GARRA_ABERTA = 35;
const unsigned int GARRA_FECHADA = 95;
const unsigned int WRITE_DELAY = 25;

bool garraAberta = false;

Servo base = Servo();
Servo ombro = Servo();
Servo cotovelo = Servo();
Servo garra = Servo();
Servo servos[3] = {base, ombro, cotovelo};

void write(Servo, unsigned int, unsigned int = WRITE_DELAY);
void posicao(const unsigned int *);
void abrirGarra();
void fecharGarra();

void setup() {
  Serial.begin(9600);

  pinMode(BASE_PIN, OUTPUT);
  pinMode(OMBRO_PIN, OUTPUT);
  pinMode(CTVL_PIN, OUTPUT);
  pinMode(GARRA_PIN, OUTPUT);

  base.attach(BASE_PIN);
  ombro.attach(OMBRO_PIN);	
  cotovelo.attach(CTVL_PIN);
  garra.attach(GARRA_PIN);

  Serial.println("P0 setup");
  base.write(P0[0]);
  ombro.write(P0[1]);
  cotovelo.write(P0[2]);
  garra.write(GARRA_ABERTA);

  Serial.println("Delay 3s");
  delay(3000);
}

void loop() {
  abrirGarra();
  Serial.println("P1");
  posicao(P1);
  delay(1000);

  Serial.println("P2");
  posicao(P2);
  delay(1000);
  fecharGarra();

  Serial.println("P1");
  posicao(P1);
  delay(1000);

  Serial.println("P3");
  posicao(P3);
  delay(1000);

  Serial.println("P4");
  posicao(P4);
  delay(1000);

  abrirGarra();
  delay(1000);

  Serial.println("P3");
  posicao(P3);
  delay(1000);

  Serial.println("P1");
  posicao(P1);
  delay(1000);
}

void write(Servo servo, unsigned int angle, unsigned int writeDelay){
  unsigned int lastWrite = servo.read();

  if (angle == lastWrite){
    return;
  }
  
  if (lastWrite < angle){
    for (unsigned int i = lastWrite; i <= angle; i++ ){
      servo.write(i);
      delay(writeDelay);
    }

    return;
  }

  if (lastWrite > angle){
    for (unsigned int i = lastWrite; i >= angle; i-- ){
      servo.write(i);
      delay(writeDelay);
    }

    return;
  }
}

void posicao(const unsigned int p[3]){
  for (int i = 0; i <= 2; i++){
    Servo s = servos[i];
    write(s, p[i]);
  }
}

void fecharGarra(){
  if (!garraAberta){
    return;
  }

  write(garra, GARRA_FECHADA);
  garraAberta = false;
}

void abrirGarra(){
  if (garraAberta){
    return;
  }

  write(garra, GARRA_ABERTA);
  garraAberta = true;
}