#include <BasicEncoder.h>
#include <TimerOne.h>

BasicEncoder encoder(2, 3);

void timerService() {
    encoder.service();
}

void encoderSetup() {
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerService);
    encoder.set_reverse();

    pinMode(BUTTON_PIN, INPUT);
}

bool buttonRead() {
    static bool button_last_state = false;
    static bool button_state = false;
    static uint32_t button_last_debounce = 0;
    bool ret = false;

    bool state = !digitalRead(BUTTON_PIN);

    if (state != button_last_state) {
        button_last_debounce = millis();
    }

    if ((millis() - button_last_debounce) > 50) { // 50ms debounce
        if (state != button_state) {
            button_state = state;

            if (button_state) {
                ret = true;
            }
        }
    }

    button_last_state = state;
    return ret;
}

void encoderLoop() {
    // button
    if (buttonRead()) {
#if defined(DEBUG)
        Serial.println(F("Button press"));
#endif
        displayMenu();
        return;
    }

    // encoder
    if (encoder.get_change()) {
        int counter = encoder.get_count();
        encoder.reset();

#if defined(DEBUG)
        Serial.print(F("Encoder change:"));
        Serial.println(counter);
#endif

        displayNext(counter);
    }
}