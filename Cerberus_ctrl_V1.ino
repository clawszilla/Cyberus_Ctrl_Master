#include <Arduino.h>

#include <ProTrinketHidCombo.h> //Setup V-USB, Keyboard, and Mouse library

//4*4 Keypad pin setup
int colPins[] = {9, 10, 11, 12};  //select pins, driven LOW to select col => btn active LOW
int rowPins[] = {6, 5, 4, 3, 1};      //signal pins, read LOW means btn pressed
//left joystick pins A0, A1
//right joystick pins A6, A7
int const rows = sizeof(rowPins)/sizeof(int);
int const cols = sizeof(colPins)/sizeof(int);

//array for prev btn states to detect distinct presses and releases
int key_prev[rows][cols] = {
  { 1, 1, 1, 1},
  { 1, 1, 1, 1},
  { 1, 1, 1, 1},
  { 1, 1, 1, 1},
  { 1, 1, 1, 1}
};

//misc UI definitions
int const modes = 3;    //number of modes/different keysets
int mode = 1;           //curent mode
int modeBtn = 8; //btn to toggle through modes
int modeBtnPrev = 1;
int ledPins[] = {13, A1, A0};  //LEDs to indicate current mode, may need: {13,14,15}
int leds = sizeof(ledPins)/sizeof(int);

//---------------->Button Layout as on Device<---------------//
//Row 1----->{{^},{>},{Tri},{Cir}},
//Row 2----->{{<},{v},{Sqr},{Ex}},
//Row 3----->{{O1},{O2},{O3},{O4}},
//Row 4----->{{O5},{O6},{O7},{O8}},
//Row 5----->{{O9},{1O},{11},{12}},

//Keypad definitions (currently empty) could introduce a set of different modes
int usb_codes[modes][rows][cols][6] = {
//Keyset 1: Painting
  {      // \/ col; {modifier,keycode1,keycode2,keycode3,keycode4,keycode5}
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},    // <-row
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}}
  },
//Keyset 2: 3D Modeling
  {      // \/ col; {modifier,keycode1,keycode2,keycode3,keycode4,keycode5}
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},    // <-row
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}}
  },
//Keyset 3: Minecraft
  {      // \/ col; {modifier,keycode1,keycode2,keycode3,keycode4,keycode5}
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},    // <-row
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}},
    {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}}
  }
};


//BEGIN void setup
void setup() {

    for(int i=0; i<cols; i++){        //set data direction for keypad
      pinMode(colPins[i], OUTPUT);    //cols: select, drive low to select
      digitalWrite(colPins[i], HIGH);
    }

    for(int i=0; i<rows; i++){        //rows: sense/read, active low
      pinMode(rowPins[i], INPUT_PULLUP);
    }

    pinMode(modeBtn, INPUT_PULLUP);   //mode btn


    for(int i=0; i<leds; i++){        //mode LEDs initialization & startup animation
      pinMode(ledPins[i], OUTPUT);
      digitalWrite(ledPins[i], HIGH);
      delay(200);
      digitalWrite(ledPins[i], LOW);
    }

// Initialize USB
    TIMSK0&=!(1<<TOIE0);              //was in example code as a bug fix, may not be needed?
    TrinketHidCombo.begin();

    while(!TrinketHidCombo.isConnected()){
      TrinketHidCombo.poll();
    }                                 //END void setup
}

void loop() {                                     //BEGIN void loop

    TrinketHidCombo.poll();                       //poll USB
                                                  //poll inputs
    if(digitalRead(modeBtn) == 0 && modeBtnPrev == 1){
      modeBtnPrev = 0;                            //poll mode button
      mode += 1;                                  //
      if(mode >= modes){                          //
      mode = 0;                                   //
      }                                           //

      for(int i=0; i<leds; i++){                  //change LEDs
        digitalWrite(ledPins[i], LOW);            //turn off all LEDs
      }

      if(TrinketHidCombo.isConnected()){          //turn on current LED if USB connected. prevents LEDs on if PC isn't
        analogWrite(ledPins[mode], 64);           //
      }                                           //
    }                                             //

    else if (digitalRead(modeBtn) == 1 && modeBtnPrev == 0){
      modeBtnPrev = 1;
    }
                                                  //BEGIN cycle through cols
    for(int i=0; i<cols; i++){
      digitalWrite(colPins[i], LOW);              //enable col (active low)

                                                  //--->BEGIN poll rows
      for(int j=0; j<rows; j++){                  //read state of current btn
        int but = digitalRead(rowPins[j]);        //Button Under Test

        if(but == 0 && key_prev[j][i] == 1){      //btn was just pressed (0), which means prev state was 1 (unpressed)
          key_prev[j][i] = 0;                     //set new prev state

          executeKeystroke(usb_codes[mode][j][i]);//execute keystroke
        }

        else if(but == 1 && key_prev[j][i] == 0){ //key release
          key_prev[j][i] = 1;                     //set new prev state
      }
    }                                             //--->END poll rows
    digitalWrite(colPins[i], HIGH);               //disable col (inative high)
    delayMicroseconds(250);                       //software debounce
  }                                               //END cycle thrugh cols


void executeKeystroke(int* keycombo){             //differentiate between keycode and multimedia keycode
  if(keycombo[1] < 110){
// || keycombo[1] == 224 || keycombo[1] == 225 || keycombo[1] == 227 || keycombo[1] == 228 || keycombo[1] == 230){
    TrinketHidCombo.pressKey(keycombo[0], keycombo[1]);
    TrinketHidCombo.pressKey(0, 0);               //release button(s)
  }
  else if(keycombo[1] >= 110 && keycombo[1] < 235){
    TrinketHidCombo.pressMultimediaKey(keycombo[1]);
    TrinketHidCombo.pressMultimediaKey(0);        //release button(s)
  }
}

  void delayMs(unsigned int ms){
    for (int i = 0; i < ms; i++) {
      delayMicroseconds(1000);
    }
  }
}                                                 //END void loop
//
//
//
//
//
