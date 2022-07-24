#include <Arduino.h>

typedef enum DISPLAY_MENU {
    DISPLAY_BASE_MENU,
    DISPLAY_BASE_PROFILE,
    DISPLAY_SET_CUSTOM,
    DISPLAY_PROFILE1,
    DISPLAY_PROFILE2,
    DISPLAY_CUSTOM
} display_current_menu_t;
display_current_menu_t display_current_menu = DISPLAY_BASE_MENU;


int display_step = 0;
bool display_menu_change = true;

uint16_t custom_temp = 180;


#include "temperature.h"
#include "display.h"
#include "user_control.h"
#include "pid.h"

void setup() {
#if defined(DEBUG)
    Serial.begin(115200);
    Serial.println("Start");
#endif
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    encoderSetup();
    displaySetup();
    pidSetup();
}


void loop() {
    tempLoop();
    encoderLoop();
    displayLoop();
    //pidLoop();
}