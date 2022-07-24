#include "Hysteresis.h"
#include "MovingAverage.h"
#include <NTC_Thermistor.h>

Hysteresis <int32_t> hysteresis(100);
MovingAverage <int32_t, 8> filter;

float current_temperature = -100;
uint32_t temp_interval = 0;

NTC_Thermistor thermistor(
    SENSOR_PIN,
    REFERENCE_RESISTANCE,
    NOMINAL_RESISTANCE,
    NOMINAL_TEMPERATURE,
    B_VALUE
);

void tempLoop() {
    if (millis() - temp_interval >= DISPLAY_INTERVAL) {
        const double celsius = thermistor.readCelsius();

        current_temperature = hysteresis.add(filter.add(celsius * 100)) / 100;

#if defined(DEBUG)
        Serial.print("Temperature: ");
        Serial.print(current_temperature);
        Serial.println("*C");
#endif
        temp_interval = millis();

        if (display_current_menu >= DISPLAY_PROFILE1) {
            display_menu_change = true;
        }
    }
}