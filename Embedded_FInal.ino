#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

int startRightMotors = 22;
int rightMotorPin1 = 16; // Front right motor
int rightMotorPin2 = 17; // Bottom right motor
int startLeftMotors = 23;
int leftMotorPin1 = 18; // Front left motor
int leftMotorPin2 = 19; // Bottom left motor

#define MAX_MOTOR_SPEED 255
const int PWMFreq = 1000;
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

int headlightPin = 25; // GPIO pin for headlights
bool headlightsOn = false; // State of the headlights

void rotateMotor(int motorSpeed, int motorPin1, int motorPin2, int speedChannel) {
    digitalWrite(motorPin1, motorSpeed > 0 ? HIGH : LOW);
    digitalWrite(motorPin2, motorSpeed < 0 ? HIGH : LOW);
    ledcWrite(speedChannel, abs(motorSpeed));
}

void setUpPinModes() {
    pinMode(headlightPin, OUTPUT);
    digitalWrite(headlightPin, LOW); // Headlights initially off

    int motorPins[] = {startRightMotors, rightMotorPin1, rightMotorPin2, startLeftMotors, leftMotorPin1, leftMotorPin2};
    for (int pin : motorPins) {
        pinMode(pin, OUTPUT);
    }

    ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
    ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);
    ledcAttachPin(startRightMotors, rightMotorPWMSpeedChannel);
    ledcAttachPin(startLeftMotors, leftMotorPWMSpeedChannel);

    rotateMotor(0, rightMotorPin1, rightMotorPin2, rightMotorPWMSpeedChannel);
    rotateMotor(0, leftMotorPin1, leftMotorPin2, leftMotorPWMSpeedChannel);
}

void setup() {
    setUpPinModes();
    Dabble.begin("Ryan's Bluetooth RC Car"); // Start Dabble connection
}

void loop() {
    Dabble.processInput();

    int rightMotorSpeed = 0, leftMotorSpeed = 0;

    if (GamePad.isUpPressed()) {
        rightMotorSpeed = leftMotorSpeed = MAX_MOTOR_SPEED;
    } else if (GamePad.isDownPressed()) {
        rightMotorSpeed = leftMotorSpeed = -MAX_MOTOR_SPEED;
    } else if (GamePad.isLeftPressed()) {
        rightMotorSpeed = MAX_MOTOR_SPEED;
        leftMotorSpeed = -MAX_MOTOR_SPEED;
    } else if (GamePad.isRightPressed()) {
        rightMotorSpeed = -MAX_MOTOR_SPEED;
        leftMotorSpeed = MAX_MOTOR_SPEED;
    }

    rotateMotor(rightMotorSpeed, rightMotorPin1, rightMotorPin2, rightMotorPWMSpeedChannel);
    rotateMotor(leftMotorSpeed, leftMotorPin1, leftMotorPin2, leftMotorPWMSpeedChannel);

    // Toggle headlights on and off using the Square button 
    // did not work correctly
    /*
    if (GamePad.isSquarePressed() && !headlightsOn) {
        headlightsOn = true;
        digitalWrite(headlightPin, HIGH);
    } else if (GamePad.isSquarePressed() && headlightsOn) {
        headlightsOn = false;
        digitalWrite(headlightPin, LOW);
    }*/
}
