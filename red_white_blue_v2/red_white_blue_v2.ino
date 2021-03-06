#include <SPI.h>
#include <TCL.h>

const byte LEDS = 50;

const byte FRAMES_PER_SECOND = 60;
const long INTERVAL = 1000 / FRAMES_PER_SECOND;

const byte RED = 0;
const byte WHITE = 1;
const byte BLUE = 2;

byte FULL_RED[] = {255, 0, 0};
byte FULL_WHITE[] = {255, 255, 255};
byte FULL_BLUE[] = {0, 0, 255};

long previousMillis = 0;

byte ledColor[LEDS];
byte ledLifetime[LEDS];
byte ledLifeLeft[LEDS];

void setup() {
  TCL.begin();
  TCL.setupDeveloperShield();

  byte lifetimeMultiplier = getLifetimeMultiplier();
  byte minLifetime = 2 * lifetimeMultiplier;
  byte maxLifetime = 3 * lifetimeMultiplier;

  byte i;
  for(i = 0; i < LEDS; i++) {
    ledColor[i] = random(0, 3);
    byte lifetime = random(minLifetime, maxLifetime);
    ledLifetime[i] = lifetime;
    ledLifeLeft[i] = random(0, lifetime);
  }
}

void loop() {
  unsigned long currentMillis = millis();


  
  if (currentMillis - previousMillis > INTERVAL) {
    previousMillis = currentMillis;
    
    float maxBrightness = (analogRead(TCL_POT4) >> 2) / 255.0;
    long randomRate = pow(analogRead(TCL_POT1), 1.5) + 1;

    byte lifetimeMultiplier = getLifetimeMultiplier();
    byte minLifetime = 2 * lifetimeMultiplier;
    byte maxLifetime = 3 * lifetimeMultiplier;
    
    TCL.sendEmptyFrame();
    
    byte i;
    for(i = 0; i < LEDS; i++) {
      byte color = ledColor[i];
      byte lifeLeft = ledLifeLeft[i];

      if (random(100000) < randomRate) {
        float brightness = random(50, 101) / 100.0;
        sendColorAndBrightness(FULL_WHITE, brightness);
      } else {
        float brightness = 1.0 * lifeLeft  / ledLifetime[i] * maxBrightness;
  
        switch(color) {
        case RED :
        sendColorAndBrightness(FULL_RED, brightness);
        break;
        case WHITE :
        sendColorAndBrightness(FULL_WHITE, brightness);
        break;
        case BLUE :
        sendColorAndBrightness(FULL_BLUE, brightness);
        break;
        }
      }

      if (lifeLeft == 0) {
        ledColor[i] = (color + random(1, 3)) % 3;
        byte lifetime = random(minLifetime, maxLifetime);
        ledLifetime[i] = lifetime;
        ledLifeLeft[i] = lifetime;
      } else {
        ledLifeLeft[i]--;
      }
    }
   
    
    TCL.sendEmptyFrame();
  }
}

void sendColorAndBrightness(byte color[3], float brightness) {
  TCL.sendColor(color[0] * brightness, color[1] * brightness, color[2] * brightness);
}

byte getLifetimeMultiplier() {
  return (analogRead(TCL_POT3) >> 2) / 3 + 1;
}

