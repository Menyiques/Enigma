#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4,5,6,7);

#define PINLED 3
#define NUM_LEDS 26
#define BRIGHTNESS 100
#define DELAY 300
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PINLED, NEO_GRB + NEO_KHZ800);
uint8_t ledmatrix[26]={9,20,22,11,6,12,13,14,1,15,16,17,19,18,0,25,8,5,10,4,2,21,7,23,24,3};
uint8_t nRotors=11;// Reflector B + I + II + III
String  rotorLabel[9]={"    "," I  "," II ","III "," IV "," V  "," VI ","VII","VIII"};
int8_t  rotorSetup[4][3]={{0,0,0},{1,0,0},{2,0,0},{3,0,0}}; //Rotor,RingStellum,CurrentPos
uint8_t rotorDefinition[9][27]={//26 wires + notch. If Notch is 99=>its a double Notch (M and Z)
       {24,16,18,4 ,12,13,5 ,22,7 ,14,3 ,21,2 ,23,24,19,14,10,13,6 ,8 ,1 ,25,12,2 ,20,0 },//0 is reflector B
       {4 ,9 ,10,2 ,7 ,1 ,23,9 ,13,16,3 ,8 ,2 ,9 ,10,18,7 ,3 ,0 ,22,6 ,13,5 ,20,4 ,10,17},//I
       {0 ,8 ,1 ,7 ,14,3 ,11,13,15,18,1 ,22,10,6 ,24,13,0 ,15,7 ,20,21,3 ,9 ,24,16,5 ,5 },//II
       {1 ,2 ,3 ,4 ,5 ,6 ,22,8 ,9 ,10,13,10,13,0 ,10,15,18,5 ,14,7 ,16,17,24,21,18,15,22},//III
       {4 ,17,12,18,11,20,3 ,19,16,7 ,10,23,5 ,20,9 ,22,23,14,1 ,13,16,8 ,6 ,15,24,2 ,10},//IV
       {21,24,25,14,2,3,13,17,12,6,8,18,1,20,23,8,10,5,20,16,22,19,9,7,4,11,26},//V
       {9,14,4,18,10,15,6,24,16,7,17,19,1,20,11,2,13,19,8,25,3,16,12,5,21,23,99},//VI
       {13,24,7,4,2,12,22,16,4,15,8,11,15,1,6,16,10,17,3,18,21,9,14,19,5,20,99},//VII
       {5,9,14,4,15,6,17,7,20,18,25,7,3,16,11,2,10,21,12,3,19,13,24,1,8,22,99},//VIII
       }; 

uint8_t inverseRotorDefinition[9][26]={//Reverse wire for convenience
       {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//0 is reflector B
       {6,5,4,23,4,2,1,18,13,10,9,7,10,3,2,22,9,20,0,8,3,13,9,7,10,16},//I
       {0,18,13,1,5,9,15,22,3,8,7,1,24,20,16,21,0,11,14,6,13,24,10,15,3,7},//II
       {7,1,22,2,15,3,14,4,18,5,16,6,17,0,15,8,18,9,21,10,24,10,5,13,10,13},//III
       {19,2,6,8,4,14,13,20,23,3,16,22,15,23,12,11,7,5,17,1,10,18,24,9,16,20},//IV
       {10,25,4,18,7,9,2,20,3,16,11,23,20,1,19,6,22,14,8,13,12,21,5,8,17,24},//V
       {8,17,5,13,19,24,4,20,12,9,19,16,6,1,10,14,7,2,25,21,15,18,23,3,16,11},//VI
       {10,15,22,5,9,16,2,4,17,7,14,18,4,13,1,21,19,12,8,20,6,3,11,16,15,24},//VII
       {10,18,20,16,12,5,8,4,13,25,9,6,21,19,7,3,14,2,7,15,24,22,3,17,1,11},//VIII
       }; 

uint8_t plugBoard[20]={99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99};


String mode="";

 
void setup() {
Serial.begin(9600);
lcd.begin(16, 2);

pinMode(A0,OUTPUT);
pinMode(A1,OUTPUT);
pinMode(A2,OUTPUT);
pinMode(A3,OUTPUT);
pinMode(A4,OUTPUT);
pinMode(A5,OUTPUT);

pinMode(A6,INPUT_PULLUP);
pinMode(A7,INPUT_PULLUP);
pinMode(A8,INPUT_PULLUP);
pinMode(A9,INPUT_PULLUP);
pinMode(A10,INPUT_PULLUP);
pinMode(A11,INPUT_PULLUP);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  hello();
  ledOff();
  displayRotors();
  
mode="RotorsSetup";
}

void hello(){
hello_letra('E');
hello_letra('N');
hello_letra('I');
hello_letra('G');
hello_letra('M');
hello_letra('A');
}
void hello_letra(char l){
  ledOn(l,255,255,255);lcd.setCursor(1,0);lcd.print(l);lcd.setCursor(7,0);lcd.print(l);lcd.setCursor(13,0);lcd.print(l); delay(DELAY*2);ledOff();      
  }

void loop() {

//------------------------------------------------------------------------------------------------
// ROTORS SETUP MODE 'R'

if (mode=="RotorsSetup"){

displayRotorsSetup();

ledOn('R',255,0,0);
ledOn('O',255,0,0);
ledOn('T',255,0,0);

if (readKeyboard()=='R'){
    ledOff();
    lcdCls();
    mode="RingStellum";
    delay(DELAY);
    }  

if (readKeyboard()=='P'){
    ledOff();
    lcdCls();
    mode="PlugBoard";
    delay(DELAY);
    }  

if ((readKeyboard()=='E')&&(rotorSetup[1][0]!=0)&&(rotorSetup[2][0]!=0)&&(rotorSetup[3][0]!=0)){
    ledOff();
    lcdCls();
    mode="Enigma";
    delay(DELAY);
    }

if (readKeyboard()=='1'){
  nextRotor(1);    
  while((rotorSetup[2][0]==rotorSetup[1][0]||rotorSetup[3][0]==rotorSetup[1][0])&&rotorSetup[1][0]!=0){
    nextRotor(1);    
    }
    delay(DELAY);
  }
if (readKeyboard()=='4'){
  prevRotor(1);    
  while((rotorSetup[2][0]==rotorSetup[1][0]||rotorSetup[3][0]==rotorSetup[1][0])&&rotorSetup[1][0]!=0){
    prevRotor(1);    
    }
    delay(DELAY);
  }
if (readKeyboard()=='2'){
  nextRotor(2);    
  while((rotorSetup[1][0]==rotorSetup[2][0]||rotorSetup[3][0]==rotorSetup[2][0])&&rotorSetup[2][0]!=0){
    nextRotor(2);    
    }
    delay(DELAY);
  }
if (readKeyboard()=='5'){
  prevRotor(2);    
  while((rotorSetup[1][0]==rotorSetup[2][0]||rotorSetup[3][0]==rotorSetup[2][0])&&rotorSetup[2][0]!=0){
    prevRotor(2);    
    }
    delay(DELAY);
  }
if (readKeyboard()=='3'){
  nextRotor(3);    
  while((rotorSetup[1][0]==rotorSetup[3][0]||rotorSetup[2][0]==rotorSetup[3][0])&&rotorSetup[3][0]!=0){
    nextRotor(3);    
    }
    delay(DELAY);
  }
if (readKeyboard()=='6'){
  prevRotor(3);    
  while((rotorSetup[1][0]==rotorSetup[3][0]||rotorSetup[2][0]==rotorSetup[3][0])&&rotorSetup[3][0]!=0){
    prevRotor(3);    
    }
    delay(DELAY);
  }

}
//END ROTORS SETUP MODE

//------------------------------------------------------------------------------------------------

//MODE ENIGMA
if (mode=="Enigma"){ //Mode "Enigma"

displayRotors();

  
  if (readKeyboard()=='1'){nextRotorPos(1);} else
if (readKeyboard()=='4'){prevRotorPos(1);} else
if (readKeyboard()=='2'){nextRotorPos(2);} else
if (readKeyboard()=='5'){prevRotorPos(2);} else
if (readKeyboard()=='3'){nextRotorPos(3);} else
if (readKeyboard()=='6'){prevRotorPos(3);} else
if (readKeyboard()!=' '){
  char key=readKeyboard();
  nextRotorPos(3);
  uint8_t c=key;
  c=c-65;

//Plugboard
for (uint8_t n=0;n<10;n++){
  char l1=plugBoard[n*2];
  char l2=plugBoard[n*2+1];
  if ((l1==c)&&(l2!=' ')){
    c=l2;
  }else if((l2==c)&&(l1!=' ')){
    c=l1;
  }
}  
Serial.print(char(c+65));
uint8_t tipo=rotorSetup[3][0];
uint8_t ring=rotorSetup[3][1];
uint8_t rotPos=rotorSetup[3][2];
uint8_t desp=rotorDefinition[tipo][(26+c+rotPos-ring)%26];
c=(c+desp)%26;
Serial.print(char(c+65));
tipo=rotorSetup[2][0];
ring=rotorSetup[2][1];
rotPos=rotorSetup[2][2];
desp=rotorDefinition[tipo][(26+c+rotPos-ring)%26];
c=(c+desp)%26;
Serial.print(char(c+65));
tipo=rotorSetup[1][0];
ring=rotorSetup[1][1];
rotPos=rotorSetup[1][2];
desp=rotorDefinition[tipo][(26+c+rotPos-ring)%26];
c=(c+desp)%26;
Serial.print(char(c+65));
c=(c+rotorDefinition[0][c])%26;
Serial.print(char(c+65));
tipo=rotorSetup[1][0];
ring=rotorSetup[1][1];
rotPos=rotorSetup[1][2];
desp=inverseRotorDefinition[tipo][(26+c+rotPos-ring)%26];
c=(26+c-desp)%26;
Serial.print(char(c+65));
tipo=rotorSetup[2][0];
ring=rotorSetup[2][1];
rotPos=rotorSetup[2][2];
desp=inverseRotorDefinition[tipo][(26+c+rotPos-ring)%26];
c=(26+c-desp)%26;
Serial.print(char(c+65));
tipo=rotorSetup[3][0];
ring=rotorSetup[3][1];
rotPos=rotorSetup[3][2];
desp=inverseRotorDefinition[tipo][(26+c+rotPos-ring)%26];
c=(26+c-desp)%26;
Serial.println(char(c+65));
for (uint8_t n=0;n<10;n++){
  char l1=plugBoard[n*2];
  char l2=plugBoard[n*2+1];
  if (l1==c){
    c=l2;
  }else if(l2==c){
    c=l1;
  }
}
Serial.println(char(c+65));

displayRotors();

while(key==readKeyboard()){
  ledOn(c+65,255,255,0);
  }  
ledOff();
}



  }


//END ENIGMA MODE

//------------------------------------------------------------------------------------------------

//MODE RINGSTELLUM

if (mode=="RingStellum"){ //Mode RingStellum "RS"

displayRotorsSetup();


ledOn('R',0,255,0);
ledOn('I',0,255,0);
ledOn('N',0,255,0);
ledOn('G',0,255,0);

if (readKeyboard()=='R'){
    ledOff();
    lcdCls();
    mode="RotorsSetup";
    delay(DELAY);
    }

if (readKeyboard()=='P'){
    ledOff();
    lcdCls();
    mode="PlugBoard";
    delay(DELAY);
    }
    
if (readKeyboard()=='E'){
    ledOff();
    lcdCls();
    mode="Enigma";
    delay(DELAY);
    }

if (readKeyboard()=='1'){rotorSetup[1][1]++; delay(DELAY);}
if (readKeyboard()=='4'){rotorSetup[1][1]--; delay(DELAY);}
if (readKeyboard()=='2'){rotorSetup[2][1]++; delay(DELAY);}
if (readKeyboard()=='5'){rotorSetup[2][1]--; delay(DELAY);}
if (readKeyboard()=='3'){rotorSetup[3][1]++; delay(DELAY);}
if (readKeyboard()=='6'){rotorSetup[3][1]--; delay(DELAY);}

for (uint8_t n=1;n<4;n++){
    if (rotorSetup[n][1]>25){rotorSetup[n][1]=0;}
    if (rotorSetup[n][1]<0){rotorSetup[n][1]=25;}
}

}

if (mode=="PlugBoard"){

displayPlugBoard();

ledOn('P',0,0,255);
ledOn('L',0,0,255);
ledOn('U',0,0,255);
ledOn('G',0,0,255);

if (readKeyboard()=='1'||readKeyboard()=='2'||readKeyboard()=='3'||readKeyboard()=='4'||readKeyboard()=='5'||readKeyboard()=='6'){
    ledOff();
    lcdCls();
    mode="RotorsSetup";
    delay(DELAY);
    }else if (readKeyboard()!=' '){
   
   char k=readKeyboard()-65;
   boolean delPlug=false;
   for (uint8_t n=0;n<20;n++){
      if (plugBoard[n]==99){
      }
      if (plugBoard[n]==k) {
          plugBoard[n]=99;
          delPlug=true;
          delay(DELAY);
       }
      }
    if (!delPlug){//está añadiendo un cable
        //ver si ya está puesto
        boolean plugged=false;
        uint8_t firstHole=99;
        for (uint8_t n=0;n<20;n++){
          if ((plugBoard[n]==99)&&(firstHole>n)){firstHole=n;}
          if (plugBoard[n]==k){plugged=true;}
        }
        if ((!plugged)&&(firstHole<20)){ plugBoard[firstHole]=k;delay(DELAY);}
        
      }
    
  
    }
  }
}

void nextRotor(uint8_t rotor){
  rotorSetup[rotor][0]++;
  if (rotorSetup[rotor][0]>nRotors-1){rotorSetup[rotor][0]=0;}  
  }
void prevRotor(uint8_t rotor){
  rotorSetup[rotor][0]--;
  if (rotorSetup[rotor][0]<0){rotorSetup[rotor][0]=nRotors-1;}  
  }

void nextRotorPos(uint8_t rotorOrder){
  
  if (rotorOrder>0){   
     rotorSetup[rotorOrder][2]++;
     uint8_t notch1=rotorDefinition[rotorSetup[rotorOrder][0]][26];
     uint8_t notch2=notch1;
     if (notch1==99){notch1=13;notch2=26;} //Its a VI,VII or VIII rotor with double notch on M and Z
      
     if ((rotorSetup[rotorOrder][2]==notch1)||(rotorSetup[rotorOrder][2]==notch2)){
      nextRotorPos(rotorOrder-1);
      }
     if (rotorSetup[rotorOrder][2]>25){
      rotorSetup[rotorOrder][2]=0;
      }  
     delay(DELAY);
     
    }
  }
void prevRotorPos(uint8_t rotor){
  rotorSetup[rotor][2]--;
  if (rotorSetup[rotor][2]<0){rotorSetup[rotor][2]=25;}  
  delay(DELAY);
  }
  
void displayRotorsSetup(){
  for (int n=1;n<4;n++){//Rotors
    lcd.setCursor((n-1)*6,0);
    String lab=rotorLabel[rotorSetup[n][0]];
    lcd.print(lab);
    lcd.setCursor(((n-1)*6),1);
    if (lab!="    "){//RingStellum
      lcd.print("R:");
      lcd.setCursor(((n-1)*6)+2,1);
      lcd.print(iToS(rotorSetup[n][1],2));
    }else{
      lcd.print("    ");
      }
  }
  }

void displayRotors(){
  for (int n=1;n<4;n++){//Rotors
    lcd.setCursor((n-1)*6,0);
    lcd.print(" "+iToS(rotorSetup[n][2],2)+"  ");
    lcd.setCursor(((n-1)*6),1);
    lcd.print("    ");
    lcd.setCursor(((n-1)*6)+1,1);
    lcd.print((char)(rotorSetup[n][2]+65));
  }
}


void displayPlugBoard(){
  uint8_t wires=0;
  for (int n=0;n<20;n++){
  uint8_t x=n%10; 
  if (n%10>3){x=x+3;} 
  if (n%10>5){x=x+3;}
  uint8_t y=n/10;
  
    
    lcd.setCursor(x,y);
    if (plugBoard[n]!=99){
      lcd.print(char(plugBoard[n]+65));
      }else{
        lcd.print(" ");
        }
    
    }
  }

  
void testleds(){
  for (int n=0;n<26;n++){
    strip.setPixelColor(n, strip.Color(255,0,0));
    strip.show();
    delay(1);
    }
    
  }
void led(char l){
   strip.setPixelColor(ledmatrix[l-65], strip.Color(255,255,0));
   strip.show();
  }
void ledOff(){
  for (int n=0;n<26;n++){
    strip.setPixelColor(n, strip.Color(0,0,0));
    }
    strip.show();
  }
char readKeyboard(){
  char tecla=' ';
  digitalWrite(A0,LOW);  digitalWrite(A1,HIGH);  digitalWrite(A2,HIGH);  digitalWrite(A3,HIGH);  digitalWrite(A4,HIGH); digitalWrite(A5,HIGH);

  if (digitalRead(A6)<1){tecla='U';}
  else if (digitalRead(A7)<1){tecla='H';}
  else if (digitalRead(A8)<1){tecla='B';}
  else if (digitalRead(A9)<1){tecla='Z';}
  else if (digitalRead(A11)<1){tecla='5';}
  
  digitalWrite(A0,HIGH);  digitalWrite(A1,LOW);

  
  if (digitalRead(A7)<1){tecla='F';}
  else if (digitalRead(A8)<1){tecla='C';}
  else if (digitalRead(A9)<1){tecla='R';}
  else if (digitalRead(A10)<1){tecla='J';}
  else if (digitalRead(A11)<1){tecla='6';}
  
  digitalWrite(A1,HIGH);  digitalWrite(A2,LOW);
  
  if (digitalRead(A7)<1){tecla='D';}
  else if (digitalRead(A8)<1){tecla='X';}
  else if (digitalRead(A9)<1){tecla='E';}
  else if (digitalRead(A10)<1){tecla='L';}
  else if (digitalRead(A11)<1){tecla='2';}

  digitalWrite(A2,HIGH);  digitalWrite(A3,LOW);
  
  if (digitalRead(A6)<1){tecla='I';}
  else if (digitalRead(A7)<1){tecla='A';}
  else if (digitalRead(A8)<1){tecla='P';}
  else if (digitalRead(A9)<1){tecla='Q';}
  else if (digitalRead(A10)<1){tecla='M';}
  else if (digitalRead(A11)<1){tecla='1';}
  
  digitalWrite(A3,HIGH);  digitalWrite(A4,LOW);
  
  if (digitalRead(A7)<1){tecla='G';}
  else if (digitalRead(A8)<1){tecla='V';}
  else if (digitalRead(A9)<1){tecla='T';}
  else if (digitalRead(A10)<1){tecla='K';}
  else if (digitalRead(A11)<1){tecla='3';}
  
  digitalWrite(A4,HIGH);  digitalWrite(A5,LOW);
  
  if (digitalRead(A6)<1){tecla='O';}
  else if (digitalRead(A7)<1){tecla='S';}
  else if (digitalRead(A8)<1){tecla='Y';}
  else if (digitalRead(A9)<1){tecla='W';}
  else if (digitalRead(A10)<1){tecla='N';}
  else if (digitalRead(A11)<1){tecla='4';}

  return tecla;
  }

void ledOn(char c,uint8_t r,uint8_t g,uint8_t b){
    strip.setPixelColor(ledmatrix[c-65], strip.Color(r,g,b));
    strip.show();
}

void lcdCls(){
  lcd.setCursor(0,0); lcd.print("                ");
  lcd.setCursor(0,1); lcd.print("                ");
  lcd.setCursor(0,0);
  }
  String iToS(uint8_t i, uint8_t len){
    return((String)i+"                ").substring(0,len);
    }
