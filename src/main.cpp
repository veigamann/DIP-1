#include <Arduino.h>
#include <Servo.h>

#define POT A0
#define BASE_PIN 3 // 180 = esquerda; 0 = direita;
#define OMBRO_PIN 5 // 40 = trás; 150 = frente;
#define CTVL_PIN 6 // 40 = em baixo; 110 = em cima;
#define GARRA_PIN 9 // 180 = fechada; 50 = 100% aberta (90 graus);

#define EQP_1_PIN 2
#define EQP_3_PIN 7
#define EQP_4_PIN 8

#define PC_PIN 10

const unsigned int WRITE_DELAY = 10;
const unsigned int P0[4] = {90, 75, 75, 35}; // 90 graus em cima, garra aberta; OBS.: garra talvez seja desnecessário no array
const unsigned int P1[4] = {90, 75, 75, 35};
const unsigned int P2[4] = {90, 75, 75, 35};
const unsigned int P3[4] = {90, 75, 75, 35};
const unsigned int P4[4] = {90, 75, 75, 35};
const unsigned int P5[4] = {90, 75, 75, 35};
const unsigned int P6[4] = {90, 75, 75, 35};

Servo base = Servo();
Servo ombro = Servo();
Servo cotovelo = Servo();
Servo garra = Servo();
Servo servos[4] = {base, ombro, cotovelo, garra};

bool garraAberta = false;

void setup() {
  Serial.begin(9600);

  pinMode(POT, INPUT);
  pinMode(BASE_PIN, OUTPUT);
  pinMode(OMBRO_PIN, OUTPUT);
  pinMode(CTVL_PIN, OUTPUT);
  pinMode(GARRA_PIN, OUTPUT);

  base.attach(BASE_PIN);
  ombro.attach(OMBRO_PIN);	
  cotovelo.attach(CTVL_PIN);
  garra.attach(GARRA_PIN);

  garra.write(35); // garra inicia aberta; isso é importante para a função write() (por conta do servo.read());
  posicao(P0); // home
}

void loop() {
  bool sinalEquipe1 = digitalRead(EQP_1_PIN);
  if (!sinalEquipe1){
    return;
  }

  bool temPeca1 = false;
  while (!temPeca1){
    temPeca1 = digitalRead(PC_PIN);
  }

  bool peca1Entregue = pegarPeca1();

  bool temPeca2 = false;
  while (!temPeca2){
    temPeca2 = digitalRead(PC_PIN);
  }

  bool peca2Entregue = pegarPeca2();

  // retorna pro home
  posicao(P0);
  delay(1000);
}

void write(Servo servo, unsigned int angle, unsigned int delayMs = WRITE_DELAY){
  int lastWrite = servo.read();

  if (angle == lastWrite){
    return;
  }
  
  if (lastWrite < angle){
    int last = lastWrite;
    for (int i = last; i <= angle; i++ ){
      servo.write(i);
      delay(delayMs);
    }

    return;
  }

  if (lastWrite > angle){
    int last = lastWrite;
    for (int i = last; i >= angle; i-- ){
      servo.write(i);
      delay(delayMs);
    }

    return;
  }
}

void posicao(const unsigned int p[4], unsigned int delayMs = 1000){
  for (int i = 0; i <= 4; i++){
    Servo s = servos[i];
    write(s, p[i]);
  }

  delay(delayMs);
}

void fecharGarra(unsigned int delayMs = WRITE_DELAY){
  if (!garraAberta){
    return;
  }

  write(garra, 180, delayMs);
  garraAberta = false;
}

void abrirGarra(unsigned int delayMs = WRITE_DELAY){
  if (garraAberta){
    return;
  }

  write(garra, 35, delayMs);
  garraAberta = true;
}

bool sinalizaEquipe(unsigned int equipePin, unsigned int delayMs = WRITE_DELAY){
  bool retorno = false;

  digitalWrite(equipePin, HIGH);
  delay(delayMs);
  digitalWrite(equipePin, LOW);

  while (!retorno){
    retorno = digitalRead(equipePin);
  }
}

bool pegarPeca1(){
  posicao(P1);
  posicao(P2);

  fecharGarra();

  posicao(P1);
  posicao(P3);
  posicao(P4);
  abrirGarra();
  posicao(P3);

  // sinaliza equipe 3 que tem peça 1
  bool retorno = sinalizaEquipe(EQP_3_PIN);
  return retorno;
}

bool pegarPeca2(){
  posicao(P5);
  delay(1000);

  posicao(P6);
  delay(1000);

  fecharGarra();
  delay(1000);

  posicao(P5);
  delay(1000);

  posicao(P3);
  delay(1000);

  posicao(P4);
  delay(1000);

  abrirGarra();
  delay(1000);

  posicao(P3);
  delay(1000);

  // sinaliza equipe 3 que tem peça 2
  bool retorno = sinalizaEquipe(EQP_3_PIN);
  return retorno;
}