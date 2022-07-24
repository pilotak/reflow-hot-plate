#include <QuickPID.h>

float Input, Output, Setpoint = 30, Kp = 2, Ki = 5, Kd = 1;
const unsigned long windowSize = 5000;
unsigned long windowStartTime, nextSwitchTime;
bool relayStatus = false;
uint16_t debounce = 50;

QuickPID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd,
               myPID.pMode::pOnError,
               myPID.dMode::dOnMeas,
               myPID.iAwMode::iAwClamp,
               myPID.Action::direct);

void pidSetup() {
    myPID.SetOutputLimits(0, windowSize);
    myPID.SetSampleTimeUs(windowSize * 1000);
    myPID.SetMode(myPID.Control::automatic);
}

void pidLoop() {

}