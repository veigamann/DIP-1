#include <arduino.h>
#include <Servo.h>

#define BASE_PIN 3
#define OMBRO_PIN 5
#define CTVL_PIN 6
#define GARRA_PIN 9
#define EQP_1 2
#define EQP_3 4
#define EQP_4 7

const unsigned int P0[3] = {90, 85, 115};
const unsigned int P1[3] = {90, 115, 115};
const unsigned int P2[3] = {90, 50, 160};
const unsigned int P3[3] = {1, 115, 115};
const unsigned int P4[3] = {1, 75, 170};
const unsigned int GARRA_ABERTA = 25;
const unsigned int GARRA_FECHADA = 100;
const unsigned int WRITE_DELAY = 25;

bool garraAberta = false;

Servo base = Servo();
Servo ombro = Servo();
Servo cotovelo = Servo();
Servo garra = Servo();
Servo servos[3] = {base, cotovelo, ombro};

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
  garraAberta = true;

  Serial.println("Delay 1s");
  delay(500);
}

int DELAY = 500;

void loop() {
  posicao(P1);
  delay(DELAY);

  posicao(P2);
  delay(DELAY);

  fecharGarra();
  delay(DELAY);

  posicao(P1);
  delay(DELAY);

  posicao(P3);
  delay(DELAY);

  posicao(P4);
  delay(DELAY);

  abrirGarra();
  delay(DELAY);

  posicao(P3);
  delay(DELAY);

  posicao(P1);
  delay(DELAY);
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

  Serial.println("Fechando garra");
  write(garra, GARRA_FECHADA);
  garraAberta = false;
}

void abrirGarra(){
  if (garraAberta){
    return;
  }

  Serial.println("Abrindo garra");
  write(garra, GARRA_ABERTA);
  garraAberta = true;
}