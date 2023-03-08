#include <Arduino.h>

// Define macros for input and output

// Motor pins
#define MOTOR_RIGHT_FORWARD            3  // PWM
#define MOTOR_RIGHT_BACKWARD           5  // PWM
#define MOTOR_LEFT_BACKWARD            11 // PWM
#define MOTOR_LEFT_FORWARD             9  // PWM
#define ENABLE_MOTORS                  2  // OUT
// DRV8833 ULT pin NC

// Infrared Receiver pin
#define IR_RECEIVER_PIN               A5 // IN
#define IR_RECEIVER_PIN_STRING       "A5"

#define NEC_protocol // Use only NEC protocol

#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes per program space

// Set-up the data to be received
#define sAddress 0xE // Address of remote is 0xE
const uint8_t CMD_STOP = 0x00;
const uint8_t CMD_FORWARD = 0x01;
const uint8_t CMD_BACKWARD = 0x02;
const uint8_t CMD_TURNLEFT = 0x03;
const uint8_t CMD_TURNRIGHT = 0x04;
const uint8_t CMD_SLIDELEFTFORWARD = 0x05;
const uint8_t CMD_SLIDERIGHTFORWARD = 0x06;
const uint8_t CMD_SLIDELEFTBACKWARD = 0x07;
const uint8_t CMD_SLIDERIGHTBACKWARD = 0x08;

#include <IRremote.hpp>

#define DELAY_AFTER_RECEIVE 10 // the delay after IR signal is received and decoded

// Sets motor speed and direction
void setMotors(int LeftMotorSpeed, int RightMotorSpeed, bool LeftMotorDirection, bool RightMotorDirection) {
    if(LeftMotorDirection == 1) { // Left motors will rotate forwards
        analogWrite(MOTOR_LEFT_FORWARD, LeftMotorSpeed);
        digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    }
    else { // Left motors will rotate backwards
        analogWrite(MOTOR_LEFT_BACKWARD, LeftMotorSpeed);
        digitalWrite(MOTOR_LEFT_FORWARD, LOW);
    }

    if(RightMotorDirection == 1) { // Right motors will rotate forwards
        analogWrite(MOTOR_RIGHT_FORWARD, RightMotorSpeed);
        digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
    }
    else { // Right motors will rotate backwards
        analogWrite(MOTOR_RIGHT_BACKWARD, RightMotorSpeed);
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

    // Enable motors
    Serial.print("Enabling motors...");
    pinMode(ENABLE_MOTORS, OUTPUT);
    pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
    pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
    pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
    pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
    digitalWrite(ENABLE_MOTORS, HIGH);
    Serial.println(" Done!");
}

void loop() {
    // Check if received signal is available and if yes, try to decode and print it.
    if(IrReceiver.decode()) {
        IrReceiver.printIRResultShort(&Serial);

        // For the motors to not react when IR signal is not fully received
        if(IrReceiver.decodedIRData.address == sAddress && (IrReceiver.decodedIRData.protocol == NEC || IrReceiver.decodedIRData.protocol == NEC2) && IrReceiver.decodedIRData.numberOfBits == 32) {
            switch (IrReceiver.decodedIRData.command) {
                case CMD_FORWARD:
                    setMotors(200,200,1,1);
                    break;
                case CMD_BACKWARD:
                    setMotors(200,200,0,0);
                    break;
                case CMD_TURNLEFT:
                    setMotors(200,200,0,1);
                    break;
                case CMD_TURNRIGHT:
                    setMotors(200,200,1,0);
                    break;
                case CMD_SLIDELEFTFORWARD:
                    setMotors(0,200,0,1);
                    break;
                case CMD_SLIDERIGHTFORWARD:
                    setMotors(200,0,1,0);
                    break;
                case CMD_SLIDELEFTBACKWARD:
                    setMotors(0,200,0,0);
                    break;
                case CMD_SLIDERIGHTBACKWARD:
                    setMotors(200,0,0,0);
                    break;
                default:
                    setMotors(0,0,0,0);
            }
        }

        delay(DELAY_AFTER_RECEIVE);
    }

    // Enable receiving of next signal
    IrReceiver.resume();
}