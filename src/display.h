#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
SSD1306AsciiAvrI2c oled;

void displaySetup() {
    oled.begin(&Adafruit128x32, OLED_ADDRESS);
    oled.setFont(ZevvPeep8x16);
    oled.clear();
}

void displayMenu() {
    oled.setInvertMode(false);

    switch (display_current_menu) {
        case DISPLAY_BASE_MENU: {
            if (display_step == 0) {
                display_current_menu = DISPLAY_BASE_PROFILE;

            } else if (display_step == 1) {
                display_current_menu = DISPLAY_SET_CUSTOM;
                oled.setInvertMode(true);
            }

            break;
        }

        case DISPLAY_BASE_PROFILE: {
            if (display_step == 0) {
                display_current_menu = DISPLAY_PROFILE1;

            } else if (display_step == 1) {
                display_current_menu = DISPLAY_PROFILE2;

            } else if (display_step == 2) { // back
                display_current_menu = DISPLAY_BASE_MENU;
            }

            break;
        }

        case DISPLAY_SET_CUSTOM: {
            display_current_menu = DISPLAY_CUSTOM;

#if defined(DEBUG)
            Serial.print(F("New custom temperature: "));
            Serial.println(custom_temp);
#endif
            break;
        }

        default:  // DISPLAY_PROFILE1 || DISPLAY_PROFILE2 || DISPLAY_CUSTOM
            display_current_menu = DISPLAY_BASE_MENU;
            break;
    }

    oled.clear();
    display_step = 0;
    display_menu_change = true;
}

void displayNext(int counter) {
    switch (display_current_menu) {
        case DISPLAY_BASE_MENU:
        case DISPLAY_BASE_PROFILE: {
            display_step += counter;

            int max = display_current_menu == DISPLAY_BASE_MENU ? 1 : 2; // DISPLAY_BASE_PROFILE has more items

            if (display_step < 0) {
                display_step = max; // jump to last item

            } else if (display_step > max) {
                display_step = 0; // jump to first item
            }

            display_menu_change = true;
            oled.clear();

            break;
        }

        case DISPLAY_SET_CUSTOM: {
            custom_temp += counter;

            if (custom_temp < 50) {
                custom_temp = 50;

#if defined(DEBUG)
                Serial.println(F("Minimum temperature is 50*C"));
#endif

            } else if (custom_temp > 280) {
                custom_temp = 280;
#if defined(DEBUG)
                Serial.println(F("Maximum temperature is 280*C"));
#endif

            } else {
                display_menu_change = true;
            }

            break;
        }

        default:
            display_menu_change = true;
            break;
    }
}

void displayLoop() {
    if (display_menu_change) {
        display_menu_change = false;

        char tmp[10];
#if defined(DEBUG)
        // Serial.println(F("Display update"));
#endif

        if (display_current_menu == DISPLAY_BASE_MENU) {
            oled.set1X();

            for (uint8_t i = 0; i < 2; i++) {
                if (display_step == i) {
                    oled.setInvertMode(true);

                } else {
                    oled.setInvertMode(false);
                }

                if (i == 0) {
                    oled.println(F(" Pajeci profil  "));

                } else if (i == 1) {
                    oled.print(F(" Vlastni        "));
                }
            }

        } else if (display_current_menu == DISPLAY_BASE_PROFILE) {
            oled.set1X();

            for (uint8_t i = 0; i < 3; i++) {
                if (display_step == i) {
                    oled.setInvertMode(true);

                } else {
                    oled.setInvertMode(false);
                }

                if (i == 0 && display_step < 2) {
                    oled.println(F(" Profil 1       "));
                }

                if (i == 1 && display_step < 2) {
                    oled.println(F(" Profil 2       "));
                }

                if (i == 2 && display_step > 1) {
                    oled.println(F(" Zpet           "));
                }
            }

        } else if (display_current_menu == DISPLAY_SET_CUSTOM) {
            int len = sprintf(tmp, " %03d ", (int)custom_temp);
            oled.set2X(); // 2x bigger font
            oled.setCursor((oled.displayWidth() - len * oled.fontWidth()) / 2 - 5, 0);// horizontal center - 5px offset
            oled.println(tmp);

        } else if (display_current_menu == DISPLAY_CUSTOM) {
            int len = sprintf(tmp, " %03d ", (int)current_temperature);
            oled.set2X(); // 2x bigger font
            oled.setCursor((oled.displayWidth() - len * oled.fontWidth()) / 2 - 3, 0); // horizontal center - 3px offset
            oled.println(tmp);

        } else { // display_current_menu == DISPLAY_PROFILE1 || display_current_menu == DISPLAY_PROFILE2
            int len = sprintf(tmp, " %03d ", (int)current_temperature);
            oled.set1X();
            oled.setCursor((oled.displayWidth() - len * oled.fontWidth()) / 2 - 3, 0);
            oled.println(tmp);
        }
    }
}