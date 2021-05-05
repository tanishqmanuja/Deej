#include <Arduino.h>

#define NUM_LEDS 3
#define NUM_SLIDERS 5

#define DISPLAY_ACTIVATION_OFFSET 10
#define DISPLAY_TIME_MS 100
#define DISPLAY_FADE_INTERVAL 500
#define NO_DISPLAY 10

const int analogInputs[NUM_SLIDERS] = {A2, A4, A1, A3, A0};
int currSliderValues[NUM_SLIDERS];
int prevSliderValues[NUM_SLIDERS];

const int ledPins[NUM_LEDS]= {3,6,9};
int ledValues[NUM_LEDS]= {0,0,0};

unsigned long displayTimeStart = 0;
boolean displayOn = false;
uint8_t displayIndex = NO_DISPLAY;

void updateSliderValues();
void sendSliderValues();
void writeDisplay(int, int, int);
void clearDisplay();
void updateDisplay();

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  for (int i = 0; i < NUM_LEDS; i++) {
     pinMode(ledPins[i],OUTPUT);
  }

  writeDisplay(0,0,0);
  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues();
  updateDisplay();
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    prevSliderValues[i] = currSliderValues[i];
    currSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)currSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void clearDisplay(){
  if(!displayOn){
    writeDisplay(0,0,0);
    return;
  }

  int x = 0;
  while(ledValues[0]>0 || ledValues[1]>0 || ledValues[2]>0){
    if(x==DISPLAY_FADE_INTERVAL){
      writeDisplay(ledValues[0],ledValues[1],ledValues[2]);
      if(ledValues[0]>0) ledValues[0] = ledValues[0] - 1;
      if(ledValues[1]>0) ledValues[1] = ledValues[1] - 1;
      if(ledValues[2]>0) ledValues[2] = ledValues[2] - 1;
      x = 0;
    } else {
      x++;
    }
  }
  writeDisplay(0,0,0);
  displayOn = false;
}

void updateDisplay(){
  if(displayOn && ((millis()-displayTimeStart) > DISPLAY_TIME_MS)){
    clearDisplay();
    displayIndex = NO_DISPLAY;
  }

  //if(displayOn) return;
  for (int i = 0; i < NUM_SLIDERS; i++) {
    if(abs(currSliderValues[i] - prevSliderValues[i]) > DISPLAY_ACTIVATION_OFFSET){
      if(displayOn && displayIndex!=i) continue;

      displayOn = true;
      displayIndex= i;
      displayTimeStart = millis();

      int volume = currSliderValues[i];

      if( volume < (1023/2) ){
        writeDisplay(map(volume,0,1023/2,255,0), map(volume,0,1023/2,0,255), 0);
      } else {
        writeDisplay(0, map(volume,1023/2,1023,255,0), map(volume,1023/2,1023,20,255));
      }
    }
  }
}

void writeDisplay(int r, int g, int b){
  ledValues[0] = r;
  ledValues[1] = g;
  ledValues[2] = b;

  analogWrite(ledPins[0],r);
  analogWrite(ledPins[1],g);
  analogWrite(ledPins[2],b);
}