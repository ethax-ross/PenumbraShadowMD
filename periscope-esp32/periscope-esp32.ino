#include "BottomLeds.h"
#include "MainLeds.h"
#include "SideLeds.h"
#include "BackLeds.h"
#include "TopLeds.h"

#define BRIGHTNESS 80   // 0-255, higher number is brighter. 
#define SPEED       25   
#define COLOR_ORDER GRB

#include "pin-map.h"

#ifndef UPPITY_SPINNER_MODE
//Command
#define MAX_COMMAND_LENGTH 10
char commandBuffer[MAX_COMMAND_LENGTH];
String commandString = "";
volatile boolean commandComplete = false;
#endif

//main
#define MAIN_NUMLEDS 9
CRGB main_leds[MAIN_NUMLEDS];
#define MAINSPEED 50

//right
#define RIGHT_NUMLEDS 9
CRGB right_leds[RIGHT_NUMLEDS];

//left
#define LEFT_NUMLEDS 9
CRGB left_leds[LEFT_NUMLEDS];

//bottom
#define BOTTOM_NUMLEDS 8
CRGB bottom_leds[BOTTOM_NUMLEDS];
#define BOTTOMLEDSPEED  200

//top
#define TOP_NUMLEDS 7
CRGB top_leds[TOP_NUMLEDS];
#define TOPSPEED 100

//back
#define BACK_NUMLEDS 3
CRGB back_leds[BACK_NUMLEDS];
#define BACKSPEED 2000

TopLeds    topLeds(top_leds,TOP_NUMLEDS);
BottomLeds bottomLeds(bottom_leds,BOTTOM_NUMLEDS);
MainLeds   mainLeds(main_leds,MAIN_NUMLEDS);
SideLeds   leftLeds(left_leds,LEFT_NUMLEDS);
SideLeds   rightLeds(right_leds,RIGHT_NUMLEDS);
BackLeds   backLeds(back_leds,BACK_NUMLEDS);

void setup() {
#ifndef UPPITY_SPINNER_MODE
  Serial.begin(9600);
  Serial.println("Starting Periscope");
#else
  pinMode(LIGHTKIT_PIN_A, INPUT_PULLUP);
  pinMode(LIGHTKIT_PIN_B, INPUT_PULLUP);
  pinMode(LIGHTKIT_PIN_C, INPUT_PULLUP);
#endif

  delay( 2000 ); // power-up safety delay
  FastLED.addLeds<WS2812, MAIN_PIN, GRB>(main_leds, MAIN_NUMLEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, RIGHT_PIN, GRB>(right_leds, RIGHT_NUMLEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, LEFT_PIN, GRB>(left_leds, LEFT_NUMLEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, BOTTOM_PIN, GRB>(bottom_leds, BOTTOM_NUMLEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, TOP_PIN, GRB>(top_leds, TOP_NUMLEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, BACK_PIN, GRB>(back_leds, BACK_NUMLEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

bool activated=true;

void loop() {

    unsigned long currentTime = millis();

#ifndef UPPITY_SPINNER_MODE
   if(commandComplete) {
    
    commandString.trim();

    Serial.print(F("Received Command: "));
    Serial.println(commandString);

    //int commandLength = commandString.length();
    commandString.toCharArray(commandBuffer, MAX_COMMAND_LENGTH);
    commandString="";
    commandComplete = false;

    if (strcmp(commandBuffer,"ON")==0){
      activated=true;
    }
    else if (strcmp(commandBuffer,"OFF")==0){
      activated=false;
    }
    
  }
#else
  int inputState = ((uint8_t(!digitalRead(LIGHTKIT_PIN_A))<<2) |
                    (uint8_t(!digitalRead(LIGHTKIT_PIN_B))<<1) |
                    (uint8_t(!digitalRead(LIGHTKIT_PIN_C))<<0));
  activated = (inputState != 1);
#endif

  if (activated){
    topLeds.update(currentTime);
    mainLeds.update(currentTime);
    rightLeds.update(currentTime);
    leftLeds.update(currentTime);
    bottomLeds.update(currentTime);
    backLeds.update(currentTime);
  }
  else {
    clearLEDS();
  }
  
  FastLED.show();
  //FastLED.delay(1000 / SPEED);  
}

void clearLEDS() {
  for(int i=0;i<MAIN_NUMLEDS;i++){main_leds[i]=CRGB::Black;}
  for(int i=0;i<LEFT_NUMLEDS;i++){left_leds[i]=CRGB::Black;}
  for(int i=0;i<RIGHT_NUMLEDS;i++){right_leds[i]=CRGB::Black;}
  for(int i=0;i<TOP_NUMLEDS;i++){top_leds[i]=CRGB::Black;}
  for(int i=0;i<BOTTOM_NUMLEDS;i++){bottom_leds[i]=CRGB::Black;}
  for(int i=0;i<BACK_NUMLEDS;i++){back_leds[i]=CRGB::Black;}
}

int l_ch=0;
void LeftLEDs(){
  static unsigned long timeLast=0;
  unsigned long elapsed=millis();
  if ((elapsed - timeLast) < MAINSPEED ) return;
  timeLast = elapsed;

  for(int i=0;i<9;i++){
    switch (l_ch) {
      case 0:
        left_leds[i] = CRGB::White;
        break;
      case 1:
        left_leds[i] = CRGB::Blue;
        break;
      case 2:
        left_leds[i] = CRGB::Red;
        break;
      case 3:
        left_leds[i] = CRGB::Black;
        break;
      default:
        left_leds[i] = CRGB::Black;
    }
  }
  l_ch++;
  if (l_ch==4){
    l_ch=0;
  }
}

int r_ch=0;
void RightLEDs(){
  static unsigned long timeLast=0;
  unsigned long elapsed=millis();
  if ((elapsed - timeLast) < MAINSPEED ) return;
  timeLast = elapsed;

  for(int i=0;i<9;i++){
    switch (r_ch) {
      case 0:
        right_leds[i] = CRGB::White;
        break;
      case 1:
        right_leds[i] = CRGB::Blue;
        break;
      case 2:
        right_leds[i] = CRGB::Red;
        break;
      case 3:
        right_leds[i] = CRGB::Black;
        break;
      default:
        right_leds[i] = CRGB::Black;
    }
  }
  r_ch++;
  if (r_ch==4){
    r_ch=0;
  }
}

int midx=0;
int mstep=1;
int br=0;
void MainLeds(){

  static unsigned long timeLast=0;
  unsigned long elapsed=millis();
  if ((elapsed - timeLast) < MAINSPEED ) return;
  timeLast = elapsed;

  main_leds[0] = CRGB::White;
  main_leds[0].fadeLightBy(br);

  for( int i = 1; i < 9; i++) {
    if (midx==i){
      main_leds[i] = CRGB::White;
    }
    else {
      main_leds[i] = CRGB::Black;
    }
  }
  midx=midx+mstep;
  if (midx==9 || midx==0){
    mstep= -mstep;
  }

  if (br>=255){
    br-=10;
  }
  else if (br<=0){
    br+=10;
  }
  
}

// 1 3 5 7
// 2 4 6 8
/*
int bt_idx=0;
void BottomLeds() {
  static unsigned long timeLast=0;
  unsigned long elapsed;
  elapsed=millis();
  if ((elapsed - timeLast) < BOTTOMLEDSPEED) return;
  timeLast = elapsed; 

  for(int i=0;i<8;i++){
    if (bt_idx==i){
      bottom_leds[i]=CRGB::Red;
    }
    else {
      bottom_leds[i]=CRGB::Black;
    }
  }
  bt_idx++;
  if (bt_idx==8){
    bt_idx=0;
  }
}
*/
int bt_arr[] ={0,2,4,6,4,2};
int *btptr=bt_arr;
int bt_cnt=1;

void BottomLeds() {
  static unsigned long timeLast=0;
  unsigned long elapsed=millis();
  if ((elapsed - timeLast) < BOTTOMLEDSPEED ) return;
  timeLast = elapsed;
  
  for(int i=0;i<8;i++){
      if (i == *btptr || i == (*btptr)+1 ){
        bottom_leds[i]=CRGB::Red;
      }
      else {
        bottom_leds[i]=CRGB::Black;
      }
  }
  bt_cnt++;


  if (bt_cnt==7){
    bt_cnt=1;
    btptr=bt_arr;
  }
  else {
    btptr++;
  }
}

int t_cnt=0;
void TopLeds() {
  static unsigned long timeLast=0;
  unsigned long elapsed=millis();
  if ((elapsed - timeLast) < TOPSPEED ) return;
  timeLast = elapsed;


  for(int i=0;i <7;i++){
    if (i==t_cnt){
      top_leds[i] = CRGB::White;
    }
    else {
      top_leds[i] = CRGB::Black;
    }
  }
  t_cnt++;
  if (t_cnt==7){
    t_cnt=0;
  }
}


void BackLeds() {
  static unsigned long timeLast=0;
  unsigned long elapsed=millis();
  if ((elapsed - timeLast) < BACKSPEED ) return;
  timeLast = elapsed;

  for(int i=0;i<3;i++){
    int a = random(0,4);
    if (a==0){
      back_leds[i] = CRGB::Red;
    }
    else if (a==1){
      back_leds[i] = CRGB::Blue;
    }
    else if (a==2){
      back_leds[i] = CRGB::White;
    }
    else {
      back_leds[i] = CRGB::Black;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef UPPITY_SPINNER_MODE
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\r' || inChar == '\n') {
      commandComplete = true;
    } else {
      commandString += inChar;
    }
  }
}
#endif
