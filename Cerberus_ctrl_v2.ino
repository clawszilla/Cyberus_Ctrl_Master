#include <Arduino.h> //setup general librarys
#include <Keyboard.h> //setup keyboard library
#include <Mouse.h> //setup mouse library

//4*5 Keypad pin setup
int colPins[] = {c1, c2, c3, c4}; //select pins, driven LOW to select col => btn active LOW
int rowPins[] = {r1, r2, r3, r4, r5}; //signal pins, read LOW means btn pressed
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

//other UI definitions
int joystick_l[] = {x, y}; //left joystick pins
int joystick_r[] = {x, y}; //right joystick pins

int trig_l = ;//left trigger pin (connected to ground)
int trig_l_key[modes][6] = {   //left trigger definitions (currently empty)
  {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}
};

int trig_r = //right trigger pin (connected to ground)
int trig_r_key[modes][6] = {   //right trigger definitions (currently empty)
  {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}
};

int led_backlit = ;//pin to control key led backlighting(if not static)

int const modes = 3;    //number of modes/different keysets
int mode = 1;           //curent mode
int modeBtn = 8; //pin for btn to toggle through modes(connected to ground)
int modeBtnPrev = 1;
int led_mode[] = {pin1, pin2, pin3};  //LEDs to indicate current mode
int leds = sizeof(ledPins)/sizeof(int);

//---------------->Button Layout as on Device<---------------//
//Row 1----->{{^},{>},{Tri},{Cir}},
//Row 2----->{{<},{v},{Sqr},{Ex}},
//Row 3----->{{O1},{O2},{O3},{O4}},
//Row 4----->{{O5},{O6},{O7},{O8}},
//Row 5----->{{O9},{1O},{11},{12}},

//Keypad definitions (currently empty)
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

    digitalWrite(led_backlit, HIGH);
    for(int i=0; i<cols; i++){        //set data direction for keypad
      pinMode(colPins[i], OUTPUT);    //cols: select, drive low to select
      digitalWrite(colPins[i], HIGH);
    }

    for(int i=0; i<rows; i++){        //rows: sense/read, active low
      pinMode(rowPins[i], INPUT_PULLUP);
    }

    pinMode(modeBtn, INPUT_PULLUP);   //mode btn as input
    pinMode(trig_r, INPUT_PULLUP);    //right trigger as input
    pinMode(trig_l, INPUT_PULLUP);    //left trigger as input
    pinMode(joystick_l[1], INPUT_PULLUP); //left joystick x pin as input
    pinMode(joystick_l[2], INPUT_PULLUP); //left joystick y pin as input
    pinMode(joystick_r[1], INPUT_PULLUP); //right joystick x pin as input
    pinMode(joystick_r[2], INPUT_PULLUP); //right joystick y pin as input

    for(int i=0; i<leds; i++){        //mode LEDs initialization & startup animation
      pinMode(ledPins[i], OUTPUT);
      digitalWrite(ledPins[i], HIGH);
      delay(200);
      digitalWrite(ledPins[i], LOW);
    }

    Keyboard.begin                    //Initialize keybaord controls
    Mouse.begin                       //Initialize mouse controls
}                                     //END void setup

void loop() {                                     //BEGIN void loop
                                                  //poll inputs
    if(digitalRead(modeBtn) == 0 && modeBtnPrev == 1){
      modeBtnPrev = 0;                            //poll mode button
      mode += 1;                                  //
      if(mode >= modes){                          //
      mode = 0;                                   //
      }                                           //

      for(int i=0; i<leds; i++){                  //change LEDs
        digitalWrite(ledPins[i], LOW);            //turn off all LEDs
        analogWrite(ledPins[mode], 64);           //
      }                                           //
    }                                             //
    else if (digitalRead(modeBtn) == 1 && modeBtnPrev == 0){
      modeBtnPrev = 1;
    }

    int trig_r_state = digitalRead(trig_r)        //read right trigger state
    if(trig_r_state == HIGH) {                    //
      Keyboard.write(trig_r_key[mode])            //if pressed, send keypress
    }

    int trig_l_state = digitalRead(trig_l)        //read left trigger state
    if(trig_l_state == HIGH) {                    //
      Keyboard.write(trig_l_key[mode])            //if pressed, send keypress
    }

    for(int i=0; i<cols; i++){                    //BEGIN cycle through cols
      digitalWrite(colPins[i], LOW);              //enable col (active low)
                                                  //--->BEGIN poll rows
      for(int j=0; j<rows; j++){                  //read state of current btn
        int but = digitalRead(rowPins[j]);        //Button Under Test

        if(but == 0 && key_prev[j][i] == 1){      //btn was just pressed (0), which means prev state was 1 (unpressed)
          key_prev[j][i] = 0;                     //set new prev state

          Keyboard.write(usb_codes[mode][j][i]);//execute keystroke
        }

        else if(but == 1 && key_prev[j][i] == 0){ //key release
          key_prev[j][i] = 1;                     //set new prev state
      }
    }                                             //--->END poll rows
    digitalWrite(colPins[i], HIGH);               //disable col (inative high)
    delayMicroseconds(250);                       //software debounce
  }                                               //END cycle thrugh cols

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
