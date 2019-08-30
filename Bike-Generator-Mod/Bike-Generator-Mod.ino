const int analogPin = A0;   // the pin that the potentiometer is attached to
const int lampCount = 6;    // the number of lights being used on each side
const int toggleSwitch = 15; // the pin to toggleSwitch between incandesent and LED lights.

bool inLedLampMode = false; //toggle to track if LED or Incandescent
int IncPins[] = {2, 3, 4, 5, 6, 7};   // an array of pin numbers to which incandescent bulb drivers are attached
int LedPins[] = {8, 9, 10, 11, 12, 13};  // an array of pin numbers to which LED bulb drivers are attached
int states[6];  // the states of the 6 lights. Lit(1) or not(0).
int clearStates[] = {1, 1, 1, 1, 1, 1};  // Array used to turn off lights on side not lit.
int thresholds[] = {250, 280, 310, 340, 360, 380}; // thresholds for each light.
int hyst = 50; //hysteresis so lights turn off at a lower level they turn on.

void setup() {
  Serial.begin(9600);
  pinMode(15, INPUT_PULLUP); //toggleSwitch - incandescents or LEDs
  for (int i = 0; i < lampCount; i++) { // setup incandesence
    pinMode(IncPins[i], OUTPUT);
    digitalWrite(IncPins[i], HIGH);
  }
  for (int i = 0; i < lampCount; i++) { // setup Leds
    pinMode(LedPins[i], OUTPUT);    
    digitalWrite(IncPins[i], LOW);
  }
  Serial.println("bike gen mod");
  
}

void loop() {
  int sensorReading = analogRead(analogPin);  //read voltage of generator

  //Serial.println(sensorReading);
  
  for (int i = 0; i < lampCount; i++) {       // set state of each light
    if (sensorReading >= thresholds[i]) {     //turn on light if above threshold
      states[i] = 0;
    } else if (sensorReading < (thresholds[i] - hyst)) {  //turn off light if below hysteresis threshold
      states[i] = 1;
    }
  }

  if (digitalRead(toggleSwitch)) {  //switched to LED lamp mode, switch is "HIGH"
    if (!inLedLampMode) {
      lights(IncPins, clearStates); //turn off all incandesent lamps
      inLedLampMode = true;         //set toggle so it doesn't clear each loop
    }
    lights(LedPins, states);        //turn on the led lamps according to states
  }
  else if (!digitalRead(toggleSwitch)) { //switched to incandescent lamp mode, switch is "LOW"
    if (inLedLampMode) {
      lights(LedPins, clearStates); //turn off all led lamps
      inLedLampMode = false;
    }
    lights(IncPins, states); //turn on the incandescent lamps according to states
  }
}

void lights(int Pins[], int states[]) { //turns lights on Pins[] according to states[]
  for (int i = 0; i < lampCount; i++) {
    if (states[i]) {
      if (Pins[i] > 7){ // LED are ON when low but Incandesent are ON when high
        digitalWrite(Pins[i], LOW);
      }else digitalWrite(Pins[i], HIGH);
    }
    else {
      if (Pins[i] > 7){
        digitalWrite(Pins[i], HIGH);
      }else digitalWrite(Pins[i], LOW);
    }
  }
}
