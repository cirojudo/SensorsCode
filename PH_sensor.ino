#include <SoftwareSerial.h> 

SoftwareSerial BLU(9,8);

const int IN1=7; 
const int IN2=6; 
const int IN3=4; 
const int IN4=5; 

const int ledDe =3;
const int ledIz =2;

char accion=' ';

bool retro=false;
bool avan = false;
bool Iz = false;
bool De = false;


void setup() {

  BLU.begin(9600);
  
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  
  pinMode(ledDe,OUTPUT);
  pinMode(ledIz,OUTPUT);

  InicioOk(); // Indica mediante los LEDS qu funciona bien.
  InicioOk();
}
void loop() {
  
  if(BLU.available()>0){
        accion = BLU.read();
        if(accion=='U'){
          Up();
          
          }
        if(accion=='S'){
          Stop();
          }
        if(accion=='B'){
          Back();
         }
        if(accion=='R'){
          Rigth();
          }
        if(accion=='L'){
          Left();
          }
       
    }
   if(retro==true){
       ModoSirena();
    }
   if(avan==true){
        EncenderLeds();
     }
   if(De==true){
     digitalWrite(ledDe,HIGH);
     digitalWrite(ledIz,LOW);
    }
   if(Iz==true){
    digitalWrite(ledDe,LOW);
    digitalWrite(ledIz,HIGH);
    }
   if(retro==false||avan==false){
         ApagarLeds();
     }

}
void InicioOk(){
   digitalWrite(ledDe,HIGH);
   digitalWrite(ledIz,HIGH);
   delay(500);
   digitalWrite(ledDe,LOW);
   digitalWrite(ledIz,LOW);
   delay(500);
   digitalWrite(ledDe,HIGH);
   digitalWrite(ledIz,HIGH);
   delay(500);
   digitalWrite(ledDe,LOW);
   digitalWrite(ledIz,LOW);
   delay(3000);
  }
void EncenderLeds(){
  digitalWrite(ledDe,HIGH);
  digitalWrite(ledIz,HIGH);
  }
void ModoSirena(){
     digitalWrite(ledDe,HIGH);
     digitalWrite(ledIz,LOW);
     delay(100);
     digitalWrite(ledDe,LOW);
     digitalWrite(ledIz,HIGH);
     delay(100);
     digitalWrite(ledDe,LOW);
     digitalWrite(ledIz,LOW);
  }
void ApagarLeds(){
  digitalWrite(ledDe,LOW);
  digitalWrite(ledIz,LOW);
}

void Up(){
  avan = true;
  retro=false;
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
 }
void Back(){
  retro=true;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);

  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  }
void Stop(){
  retro=false;
  avan=false;
  De=false;
  Iz=false;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);

  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
 }
void Rigth(){
  De=true;
  Iz=false;
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);

  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
 
}
void Left(){
  Iz=true;
  De=false;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
 
}