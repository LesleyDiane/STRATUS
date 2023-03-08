/***
 * STRATUS 1.0
 *
 * February 02, 2022 Wednesday 11:11 AM
 * ********************************************************
 * Last year, I first attempted to make a car, but it was a
 * self-driving one that avoids obstacles, or, at least that
 * was what it was meant to do. It was a big fail! The motor
 * driver that I used was inefficient. The motors and other
 * components would fall out of the chassis. The code was
 * awful and messy (I was too lazy to use comments then.
 * But now, I know better and I would use them whenever
 * necessary, like how I am utilizing them now). The servo
 * motor that controls the Ultrasonic Sensor would get stuck
 * occassionaly. I did not know then that a regular 9V
 * battery would have insufficient current to supply enough
 * power to two DC motors. I was just so frustated! However,
 * I have learned from my mistakes and I am willing to not
 * give up on this one.
 * This is for my little sister.
 * ********************************************************
 * Written By: Leslie Diane A. Monzon
 *
 * Receives and decodes IR signals sent by a joystick remote
 * then maps these signals to varying intensity and direction
 * of 4 DC Motors controlled using DRV8833.
 *
 * With RB Lighting (no G because ran out of PWM pins 😣)
 *
 * ********************************************************
 * This is one of my very first few (working (hopefully))
 * programs, so I am not that good yet.
 * ********************************************************
 * The name STRATUS is a cloud's name. I think it is the
 * lowest cloud. No, I think the lowest cloud is named
 * Nimbus, but, it's just too scary and dark so I used
 * Stratus instead which is the second lowest cloud (I think).
 * So anyway, I named the car STRATUS because I am expecting
 * to make other stuff as well, like something that can fly
 * which I will name CUMULUS, then a rocket which I will
 * name CIRRUS. See the pattern? Cumulus clouds are higher
 * than Stratus clouds, and Cirrus clouds are higher than
 * Cumulus clouds. By the same idea, NOCTILUS can be the
 * name of a vehicle that travels beyond the Solar Sytem,
 * and NEBULUS travels beyond the Milky Way Galaxy. Maybe,
 * just maybe, maybe it can maybe transport humans to another
 * habitable planet, maybe. Maybe, it may be!
 * ********************************************************
***/

#include <Arduino.h>

// Define macros for input and output

// Motor pins
#define MOTOR_RIGHT_FORWARD            3 // PWM
#define MOTOR_RIGHT_BACKWARD           5 // PWM
#define MOTOR_LEFT_BACKWARD            9 // PWM
#define MOTOR_LEFT_FORWARD             6 // PWM
#define ENABLE_MOTORS                  2 // OUT
// DRV8833 ULT pin NC

// RGB LED pins
#define LED_RED_1                      4 // OUT
#define LED_RED_2                      7 // OUT
#define LED_BLUE_1                    10 // PWM
#define LED_BLUE_2                    11 // PWM
// Green pins NC

// Infrared Receiver pin
#define IR_RECEIVER_PIN               A5 // IN
#define IR_RECEIVER_PIN_STRING       "A5"

/*** Here goes ***/

#define NEC_protocol // Use only NEC protocol

/*** I don't know what this does but the docs said it can save space ***/
#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes per program space

#define sAddress 0xE // Address of remote is 0xE

/*** I would define them as function return values, but I ***/
/*** do not know how to return two values in one function ***/
// Motor speeds
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

#include <IRremote.hpp>

#define DELAY_AFTER_RECEIVE 1000 // the delay after IR signal is received, decoded, and proper volatge is applied to motors

// Light show sequence that is performed every start-up
/*** COOLNESS ***/
void startupLightShow() {
/**************************************************** TODO ************************************************/
/**************************************************** TODO ************************************************/
/**************************************************** TODO ************************************************/
/**************************************************** TODO ************************************************/
/**************************************************** TODO ************************************************/
}

// Correlates the IR signal to whether the motors spin forward or backward
bool mapToDirection(int IRSignal) {
    bool direction; // true is forwards, false is backwards

    if((IRSignal >= 0 && IRSignal <= 119) || (IRSignal >= 160 && IRSignal <= 216)) {
        direction = true; // true is forwards
    }
    else if((IRSignal >= 120 && IRSignal <= 159) || (IRSignal >= 217 && IRSignal <= 254)) {
        direction = false; // false is backwards
    }

    return direction;
}

// Correlates the IR signal to the speed of the rotation of the motors
void mapToSpeed(int IRSignal) {
    /*** Hope math works ***/

    if(IRSignal == 0xFF) { // If joystick is not being moved, turn off motors
        digitalWrite(ENABLE_MOTORS, LOW);
    }

    bool buttonPressed = false; // If button is pressed on the joystick, motors rotate slower.

    // Determine whether button is pressed or not according to the signal
    if(IRSignal >= 0 && IRSignal <= 159) {
        buttonPressed = true;
    }
    else if(IRSignal >= 160 && IRSignal <= 254) {
        buttonPressed = false;
    }

    // If button is pressed, motor speed varies from 75 to 90.
    // Else, motor speed varies from 100 to 150.
    if (buttonPressed == true) {
    // Determine the position of joystick according to the signal and map to speed for the motors
        // Joystick is above the x-axis
        if(IRSignal >= 0 && IRSignal <= 54) { // Joystick is on second quadrant
            rightMotorSpeed = map(IRSignal, 0, 54, 100, 200); // Motors on the right will rotate faster
            leftMotorSpeed = map(IRSignal, 0, 54, 75, 150); // than motors on the left, car will turn left
        }

        else if(IRSignal >= 55 && IRSignal <= 64) { // Joystick is on positive y-axis
            rightMotorSpeed = map(IRSignal, 55, 64, 150, 220); // Motors on the right will rotate at the
            leftMotorSpeed = rightMotorSpeed; // same speed as motors on the left, car will not turn
        }

        else if(IRSignal >= 65 && IRSignal <= 119) { // Joystick is on first quadrant
            rightMotorSpeed = map(IRSignal, 65, 119, 75, 150); // Motors on the right will rotate slower
            leftMotorSpeed = map(IRSignal, 65, 119, 100, 200); // than motors on the left, car will turn right
        }

        // Joystick is below the x-axis, motor speeds will be slower relative to when it is above the x-axis
        else if(IRSignal >= 120 && IRSignal <= 136) { // Joystick is on third qudrant
            rightMotorSpeed = map(IRSignal, 120, 136, 100, 150); // Motors on the right will rotate faster
            leftMotorSpeed = map(IRSignal, 120, 136, 75, 100); // than motors on the left, car will turn left
        }

        else if(IRSignal >= 137 && IRSignal <= 142) { // Joystick is on negative y-axis
            rightMotorSpeed = map(IRSignal, 137, 142, 150, 150); // Motors on the right will rotate at the
            leftMotorSpeed = rightMotorSpeed; // same speed as motors on the left, car will not turn
        }

        else if(IRSignal >= 143 && IRSignal <= 159) { // Joystick is on fourth quadrant
            rightMotorSpeed = map(IRSignal, 143, 159, 75, 100); // Motors on the right will rotate slower
            leftMotorSpeed = map(IRSignal, 143, 159, 100, 150); // than motors on the left, car will turn right
        }
    }

    else { // If button is pressed
    // Motors will rotate slower than when the button is not pressed
        // Joystick is above the x-axis
        if(IRSignal >= 160 && IRSignal <= 185) { // Joystick is on second quadrant
            rightMotorSpeed = map(IRSignal, 160, 185, 80, 90); // Motors on the right will rotate faster
            leftMotorSpeed = map(IRSignal, 160, 185, 75, 80); // than motors on the left, car will turn left
        }

        else if(IRSignal >= 186 && IRSignal <= 190) { // Joystick is on positive y-axis
            rightMotorSpeed = map(IRSignal, 186, 190, 75, 90); // Motors on the right will rotate at the
            leftMotorSpeed = rightMotorSpeed; // same speed as motors on the left, car will not turn
        }

        else if(IRSignal >= 191 && IRSignal <= 216) { // Joystick is on first quadrant
            rightMotorSpeed = map(IRSignal, 191, 216, 75, 80); // Motors on the right will rotate slower
            leftMotorSpeed = map(IRSignal, 191, 216, 80, 90); // than motors on the left, car will turn right
        }

        // Joystick is below the x-axis, motor speeds will be slower relative to when it is above the x-axis
        else if(IRSignal >= 217 && IRSignal <= 232) { // Joystick is on third qudrant
            rightMotorSpeed = map(IRSignal, 217, 232, 75, 85); // Motors on the right will rotate faster
            leftMotorSpeed = 75; // than motors on the left, car will turn left
        }

        else if(IRSignal >= 233 && IRSignal <= 238) { // Joystick is on negative y-axis
            rightMotorSpeed = map(IRSignal, 233, 238, 75, 85); // Motors on the right will rotate at the
            leftMotorSpeed = rightMotorSpeed; // same speed as motors on the left, car will not turn
        }

        else if(IRSignal >= 239 && IRSignal <= 254) { // Joystick is on fourth quadrant
            rightMotorSpeed = 75; // Motors on the right will rotate slower than motors on the
            leftMotorSpeed = map(IRSignal, 239, 254, 75, 85); // left, car will turn right
        }
    }

/*** THAT was very TIRING and INEFFICIENT but I do not know yet what I can do to make it concise ***/
}

// Sets motor speeds and direction according to the mapped IR signal
void setMotorSpeed(int LeftMotorSpeed, int RightMotorSpeed, bool MotorDirection) {
    if(MotorDirection == true) { // Wheels will rotate forwards
        analogWrite(MOTOR_LEFT_FORWARD, LeftMotorSpeed);
        analogWrite(MOTOR_RIGHT_FORWARD, RightMotorSpeed);
        digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
        digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
    }

    else { // Wheels will rotate backwards
        analogWrite(MOTOR_LEFT_BACKWARD, LeftMotorSpeed);
        analogWrite(MOTOR_RIGHT_BACKWARD, RightMotorSpeed);
        digitalWrite(MOTOR_LEFT_FORWARD, LOW);
        digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
    }
}

void setup() {
    Serial.begin(9600); // Allows conversation between Serial Monitor and UNO at 9600 baud rate

    // To know which program is running on UNO
    Serial.print("Start ");
    Serial.print(__FILE__);
    Serial.print(" from ");
    Serial.print(__DATE__);

    // In case the interrupt driver crashes on setup, give a clue to the user what's going on.
    /*** Copied that from docs... It can crash? What even is an interrupt driver? ***/
    Serial.println(F("Enabling IRin..."));

    // Start the IR receiver, enable feedback LED
    IrReceiver.begin(IR_RECEIVER_PIN, true);

    // Let the user know which pin and protocol is being used for the IR receiver.
    Serial.print(F("Ready to receive IR signals of protocol: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.print(IR_RECEIVER_PIN_STRING);
    Serial.println("."); /*** Always add a punctuation mark after every sentence. ***/

    // Set LEDs to OUTPUT
    pinMode(LED_RED_1, OUTPUT);
    pinMode(LED_RED_2, OUTPUT);
    pinMode(LED_BLUE_1, OUTPUT);
    pinMode(LED_BLUE_2, OUTPUT);

    // Enable motors
    Serial.println("Enabling motors...");
    pinMode(ENABLE_MOTORS, OUTPUT);
    pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
    pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
    pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
    pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
    digitalWrite(ENABLE_MOTORS, HIGH);

    // Initiate start-up light sequence
    /*** For coolness, and to confirm that nothing crashed... I think ***/
    Serial.println("Initiating start-up light sequence...");
    startupLightShow();
}
// If joystick is above horizontal, motors will spin forwards, below horizontal, motors spin backwards
bool motorDirection;

void loop() {
    // Check if received signal is available and if yes, try to decode and print it.
    if(IrReceiver.decode()) {
        IrReceiver.printIRResultShort(&Serial);

        // For the motors to not react when IR signal is not fully received
        if(IrReceiver.decodedIRData.address == sAddress && IrReceiver.decodedIRData.protocol == NEC && IrReceiver.decodedIRData.numberOfBits == 32) {
        // Check the received data and perform actions according to the received address and commands

            digitalWrite(ENABLE_MOTORS, HIGH); // turn on motors

            // Determine whether the joystick is above or below the x-axis
            motorDirection = mapToDirection(IrReceiver.decodedIRData.command);

            // Determine the position of the joystick and map to values corresponding to the speed of the motors
            mapToSpeed(IrReceiver.decodedIRData.command);

            // Set motor speeds according to the position of the joystick
            setMotorSpeed(leftMotorSpeed, rightMotorSpeed, motorDirection);
        }

        delay(DELAY_AFTER_RECEIVE);
    }

    // Enable receiving of next signal
    IrReceiver.resume();
}