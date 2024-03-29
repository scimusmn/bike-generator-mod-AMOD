#define generatorPin A0  // the generator voltage divider pin
#define switchPin 15     // the switch that toggles between incandescent and LED 

int IncPins[] = {2, 3, 4, 5, 6, 7};   // an array of pin numbers to which incandescent bulb drivers are attached
int LedPins[] = {8, 9, 10, 11, 12, 13};  // an array of pin numbers to which LED bulb drivers are attached

const int lampCount = 6;    // the number of lights being used on each side
bool inLedLampMode = false; //toggle to track if LED or Incandescent
int states[6];  // the states of the 6 lights. Lit(1) or not(0).
int clearStates[] = {0, 0, 0, 0, 0, 0};  // Array used to turn off lights on side not lit.
int thresholds[] = {250, 280, 310, 340, 360, 380}; // thresholds for each light.
int hyst = 50; //hysteresis so lights turn off at a lower level they turn on.

void setup() {
  pinMode(15, INPUT_PULLUP); //switchPin - incandescents or LEDs
  for (int i = 0; i < lampCount; i++) { // setup incandescent
    pinMode(IncPins[i], OUTPUT);
    digitalWrite(IncPins[i], HIGH);  // high turns Incandescent side off 
  }
  for (int i = 0; i < lampCount; i++) { // setup Leds
    pinMode(LedPins[i], OUTPUT);
    digitalWrite(IncPins[i], LOW);  // // low turns LED side off
  }
}

void loop() {
  int voltage = analogRead(generatorPin);  //read voltage of generator
  for (int i = 0; i < lampCount; i++) {       // set state of each light
    if (voltage >= thresholds[i]) {     //turn on light if above threshold
      states[i] = 1;
    } else if (voltage < (thresholds[i] - hyst)) {  //turn off light if below hysteresis threshold
      states[i] = 0;
    }
  }

  if (digitalRead(switchPin)) {  //switched to LED lamp mode, switch is "HIGH"
    if (!inLedLampMode) {
      lights(IncPins, clearStates); //turn off all incandesent lamps
      inLedLampMode = true;         //set toggle so it doesn't clear each loop
    }
    lights(LedPins, states);        //turn on the led lamps according to states
  }
  else if (!digitalRead(switchPin)) { //switched to incandescent lamp mode, switch is "LOW"
    if (inLedLampMode) {
      lights(LedPins, clearStates); //turn off all led lamps
      inLedLampMode = false;
    }
    lights(IncPins, states); //turn on the incandescent lamps according to states
  }
}

void lights(int Pins[], int states[]) { //turns lights on Pins[] according to states[]
  
  // *** LED are ON when HIGH but Incandesent are OFF when HIGH
  
  for (int i = 0; i < lampCount; i++) {
    if (states[i]) {
      if (Pins[i] < 8) {                  // if it's incandescent
        digitalWrite(Pins[i], LOW);      // set pin LOW for on.
      } else digitalWrite(Pins[i], HIGH);  // else, it's LED, set it HIGH for on.
    }
    else {
      if (Pins[i] < 8) {                  // if it's incandescent
        digitalWrite(Pins[i], HIGH);       // set pin HIGH for off.
      } else digitalWrite(Pins[i], LOW); // else, it's LED, set it LOW for off.
    }
  }
}
