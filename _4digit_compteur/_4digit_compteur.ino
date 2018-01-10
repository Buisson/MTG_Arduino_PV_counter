#include "SevSeg.h"

SevSeg sevseg; //Instantiate a seven segment controller object
int interruptSignalMoins = 2;
int interruptSignalPlus = 3;

int valPot=0; //variable pour récupérer la tension aux bornes du potentiomètre 

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {A0, A1, 4, 5, 6, 7, 8, 9};

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  // If your display is common anode type, please edit comment above line and uncomment below line
  //sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);
  attachInterrupt(digitalPinToInterrupt(interruptSignalMoins), degat, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptSignalPlus), heal, CHANGE);
  Serial.begin(9600);
}

int vieJoueurUn = 20;
int vieJoueurDeux = 20;

unsigned long lastInterrupt;

void loop() {
  String vieJ1Str = "";
  String vieJ2Str = "";
  //sevseg.setNumber(2020, 1);
  if(vieJoueurUn<10 && vieJoueurUn>0){
    vieJ1Str = "0"+String(vieJoueurUn);
  }
  else{
    vieJ1Str = String(vieJoueurUn);
  }
  if(vieJoueurDeux<10 && vieJoueurDeux>0){
    vieJ2Str = "0"+String(vieJoueurDeux);
  }
  else{
    vieJ2Str = String(vieJoueurDeux);
  }
  String numberToDisplay = afficherChiffre(vieJ1Str + vieJ2Str);
  char buff[5];
  numberToDisplay.toCharArray(buff, 5);
  sevseg.setChars(buff);
  sevseg.refreshDisplay(); // Must run repeatedly
}

String afficherChiffre(String chiffre) {
  String myString = chiffre;
  String reverse = String(myString[3]) + String(myString[2]) + String(myString[1]) + String(myString[0]);
  return reverse;
}

void perdUnPv(int joueur) {
  switch (joueur) {
    case 1:
      vieJoueurUn--;
      break;
    case 2:
      vieJoueurDeux--;
      break;
    default:
      break;
  }
}

void gagneUnPv(int joueur) {
  switch (joueur) {
    case 1:
      vieJoueurUn++;
      break;
    case 2:
      vieJoueurDeux++;
      break;
    default:
      break;
  }
}

void heal(){
  if(millis() - lastInterrupt > 100) // we set a 10ms no-interrupts window
  {
      lastInterrupt = millis();
      valPot=analogRead(A5); //lit la tension, la convertit en valeur numérique et la stocke dans valeurPot
      if(valPot>511){
        gagneUnPv(1);
      }
      else{
        gagneUnPv(2);
      }
  }
}


void degat(){
  if(millis() - lastInterrupt > 100) // we set a 10ms no-interrupts window
  {
    lastInterrupt = millis();
    valPot=analogRead(A5); //lit la tension, la convertit en valeur numérique et la stocke dans valeurPot
    if(valPot>511){
      perdUnPv(1);
    }
    else{
      perdUnPv(2);
    }
  }
}

/// END ///
