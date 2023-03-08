#include <Arduino.h>

// Define macros for input and output

// Joystick pins
#define JOYSTICK_VERTICAL_PIN         A1 // IN
#define JOYSTICK_HORIZONTAL_PIN       A0 // IN
#define JOYSTICK_SWITCH_PIN            4 // IN

// Infrared transmitter pin
#define IR_TRANSMITTER_PIN             9 // PWM
#define IR_TRANSMITTER_PIN_STRING     "9"

// These are the voltage readings when the joystick module used is at the center
#define JOYSTICK_HORIZONTAL_ORIGIN   499
#define JOYSTICK_VERTICAL_ORIGIN     525

#define NO_LED_FEEDBACK_CODE // saves 566 bytes program space

// Set-up the data to be sent
uint16_t sAddress = 0xE; // Address used by remote is 0xE
uint8_t sCommand = 0xFF; // The hex code will be stored here
uint8_t CMD_STOP = 0x00;
uint8_t CMD_FORWARD = 0x01;
uint8_t CMD_BACKWARD = 0x02;
uint8_t CMD_TURNLEFT = 0x03;
uint8_t CMD_TURNRIGHT = 0x04;
uint8_t CMD_SLIDELEFTFORWARD = 0x05;
uint8_t CMD_SLIDERIGHTFORWARD = 0x06;
uint8_t CMD_SLIDELEFTBACKWARD = 0x07;
uint8_t CMD_SLIDERIGHTBACKWARD = 0x08;

#include <IRremote.hpp>

// delay must be greater than 5 ms, otherwise the receiver sees it as one long signal
#define DELAY_AFTER_SEND 10// See table above for conversion from joystick position to IR signal

uint8_t joystickPosition() {

    // Gives the "distance" from the joystick to the origin
    int JOYSTICK_VERTICAL = analogRead(JOYSTICK_VERTICAL_PIN) - JOYSTICK_VERTICAL_ORIGIN;
    int JOYSTICK_HORIZONTAL = analogRead(JOYSTICK_HORIZONTAL_PIN) - JOYSTICK_HORIZONTAL_ORIGIN;
    // Using these, the origin is (0,0), q1 is (-,+), q2 is (+,+), q3 is (+,-), and q4 is (-,-)


    if(abs(JOYSTICK_HORIZONTAL) < 100 && JOYSTICK_VERTICAL > 100) {sCommand = CMD_FORWARD; }
    else if(abs(JOYSTICK_HORIZONTAL) < 100 && JOYSTICK_VERTICAL < -100) { sCommand = CMD_BACKWARD; }
    else if(JOYSTICK_HORIZONTAL > 100 && JOYSTICK_VERTICAL > 100) { sCommand = CMD_SLIDELEFTFORWARD; }
    else if(JOYSTICK_HORIZONTAL < -100 && JOYSTICK_VERTICAL > 100) { sCommand = CMD_SLIDERIGHTFORWARD; }
    else if(JOYSTICK_HORIZONTAL > 100 && JOYSTICK_VERTICAL < -100) { sCommand = CMD_SLIDELEFTBACKWARD; }
    else if(JOYSTICK_HORIZONTAL < -100 && JOYSTICK_VERTICAL < -100) { sCommand = CMD_SLIDERIGHTBACKWARD; }
    else if(abs(JOYSTICK_VERTICAL) < 100 && JOYSTICK_HORIZONTAL > 100) { sCommand = CMD_TURNLEFT; }
    else if(abs(JOYSTICK_VERTICAL) < 100 && JOYSTICK_HORIZONTAL < -100) { sCommand = CMD_TURNRIGHT; }
    else { sCommand = CMD_STOP; }

    return(sCommand);
}

void setup() {
    Serial.begin(9600); // Allow sending and receiving of data from NANO to Serial Monitor at 9600 baud rate

    // To know which program is running on NANO
    Serial.print("Start ");
    Serial.print(__FILE__);
    Serial.print(" from ");
    Serial.print(__DATE__);

    // Start the IR transmitter, enable feedback LED
    IrSender.begin(IR_TRANSMITTER_PIN);

    // Let the user know which pins and protocol is being used for the IR transmitter.
    Serial.print(F("Ready to send IR signals of protocol: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.print(IR_TRANSMITTER_PIN_STRING);
    Serial.println("."); /*** "Always add a punctuation mark after every sentence" ***/

    // Let the user now what the set signal mark duration, pulse correction, and total period is.
    /*** Whatever THOSE are ***/
    IrSender.enableIROut(38); // Call it with 38 kHz to initialize the values printed below
    Serial.print(F("Send signal mark duration is "));
    Serial.print(IrSender.periodOnTimeMicros);
    Serial.print(F(" us, pulse correction is "));
    Serial.print(IrSender.getPulseCorrectionNanos());
    Serial.print(F(" ns, total period is "));
    Serial.print(IrSender.periodTimeMicros);
    Serial.println(F(" us"));

    // Set Joystick pins to input
    pinMode(JOYSTICK_VERTICAL_PIN, INPUT);
    pinMode(JOYSTICK_HORIZONTAL_PIN, INPUT);
    pinMode(JOYSTICK_SWITCH_PIN, INPUT);
}

void loop() {
    joystickPosition(); // Call the function to update the joystick position

    // Print the values that will be transmitted
    Serial.println();
    Serial.print(F("address=0x"));
    Serial.print(sAddress, HEX);
    Serial.print(F(" command=0x"));
    Serial.print(sCommand, HEX);
    Serial.println();
    Serial.println();
    Serial.flush();

    Serial.println(F("Send NEC with 8 bit address"));
    Serial.flush();
    IrSender.sendNEC(sAddress, sCommand, 0);
    delay(DELAY_AFTER_SEND);
}