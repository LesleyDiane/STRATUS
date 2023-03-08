/***
 * Infrared Joystick Remote Control for the STRATUS 1.0
 *
 * March 03, 2022 Thursday 6:42 AM
 **********************************************************
 * It may be noticed that it took a long time for me to
 * start writing this. It is because the iMAX B6 charger
 * that I bought online was a counterfeit >.< . Why do I
 * still buy stuff online when most of the things I buy
 * are either a counterfeit or of very poor quality? One
 * time, I bought Gilbert Strang's Linear Algebra book and
 * it took less than a month for the whole book to separate
 * from the cover! The seller did not even respond to my
 * messages! Anyway, so I could not get the battery charger
 * to work. (RIP P1,320 and 14 waiting days) So, I had to
 * buy a new one, which took roughly another 14 days to get
 * delivered.
 **********************************************************
 * Written By: Leslie Diane A. Monzon
 *
 * Sends Infrared signals according to the position of the
 * Joystick that can be decoded by STRATUS
 *
 * If joystick button is pressed, motors run slower
 * for precision driving
 *
 * With RGB Lighting
 * ********************************************************
 * I hope the conversion from Joystick position to Infrared
 * signal to voltage for four motors (although I connected
 * the two left motors and the two right motors so,
 * technically, I am only controlling two voltages) works.
 * ********************************************************
 *
 * **** CONVERSION FROM JOYSTICK POSITION TO IR SIGNAL ****
 * 000 - 159 will be alloted to w/o button:
 *  | 000 - 119 forward:
 *  | | 000 - 054 left
 *  | | 055 - 064 middle
 *  | | 065 - 119 right
 *  | 120 - 159 backward:
 *  | | 120 - 136 left
 *  | | 137 - 142 middle
 *  | | 143 - 159 right
 * 160 - 254 will be alloted to w/ button:
 * | 160 - 216 forward:
 * | | 160 - 185 left
 * | | 186 - 190 middle
 * | | 191 - 216 right
 * | 217 - 254 backward:
 * | | 217 - 232 left
 * | | 233 - 238 middle
 * | | 239 - 254 right
 * ********************************************************
***/


#include <Arduino.h>

// Define macros for input and output

// Joystick pins
#define JOYSTICK_VERTICAL_PIN         A1 // IN
#define JOYSTICK_HORIZONTAL_PIN       A0 // IN
#define JOYSTICK_SWITCH_PIN            4 // IN

// Infrared transmitter pin
#define IR_TRANSMITTER_PIN             9 // PWM
#define IR_TRANSMITTER_PIN_STRING     "9"

// RGB LED pins
#define LED_RED                        7 // PWM
#define LED_GREEN                      6 // PWM
#define LED_BLUE                       5 // PWM

// These are the voltage readings when the joystick module used is at the center
#define JOYSTICK_HORIZONTAL_ORIGIN   499
#define JOYSTICK_VERTICAL_ORIGIN     525

#define NO_LED_FEEDBACK_CODE // saves 566 bytes program space

// Set-up the data to be sent
uint16_t sAddress = 0xE; // Address used by car is 0xE
uint8_t sCommand = 0xFF; // 0xFF means joystick is not being used

#include <IRremote.hpp>

// delay must be greater than 5 ms, otherwise the receiver sees it as one long signal
#define DELAY_AFTER_SEND 10

// Light show sequence that is performed every start-up
void startupLightShow() {
/**************************************************** TODO ************************************************/
/**************************************************** TODO ************************************************/
/**************************************************** TODO ************************************************/
/**************************************************** TODO ************************************************/
/**************************************************** TODO ************************************************/
}

// See table above for conversion from joystick position to IR signal
int mapJoystickVoltageToIRSignal() {
    // Gives the "distance" from the joystick to the origin
    int JOYSTICK_VERTICAL = analogRead(JOYSTICK_VERTICAL_PIN) - JOYSTICK_VERTICAL_ORIGIN;
    int JOYSTICK_HORIZONTAL = analogRead(JOYSTICK_HORIZONTAL_PIN) - JOYSTICK_HORIZONTAL_ORIGIN;
    // Using these, the origin is (0,0), q1 is (-,+), q2 is (+,+), q3 is (+,-), and q4 is (-,-)

    // The maximum values JOYSTICK_VERTICAL and JOYSTICK_HORIZONTAL can obtain
    // which correspond to when the joystick is at the extremities
    int JOYSTICK_VERTICAL_MAX = 1023 - JOYSTICK_VERTICAL_ORIGIN;
    int JOYSTICK_HORIZONTAL_MAX = 1023 - JOYSTICK_HORIZONTAL_ORIGIN;

    // The return value which is the very signal transmitted by the IR LED
    int IRsignal;

    // if joystick is not being moved
    if(abs(digitalRead(JOYSTICK_HORIZONTAL)) <= 100 && abs(digitalRead(JOYSTICK_VERTICAL)) <= 100 ) {
        IRsignal = 0xFF;
    }

    // If button is pressed
    if(digitalRead(JOYSTICK_SWITCH_PIN) == 0) {
        if(JOYSTICK_VERTICAL > 100) { // Joystick is in the first two quadrants
            if(JOYSTICK_HORIZONTAL > 100){ // Joystick is in second quadrant
                // The more the joystick tends to the extremities, the higher the value of the signal
                IRsignal = map(JOYSTICK_VERTICAL + JOYSTICK_HORIZONTAL, 0, JOYSTICK_VERTICAL_MAX + JOYSTICK_HORIZONTAL_MAX, 160, 185); // forward left w/ button
            }

            else if(abs(JOYSTICK_HORIZONTAL) <= 100) { // Joystick is in positive y-axis
                IRsignal = map(JOYSTICK_VERTICAL, 101, JOYSTICK_VERTICAL_MAX, 186, 190); // forward middle w/ button
            }

            else if(JOYSTICK_HORIZONTAL < -100) { // Joystick is in first quadrant
                IRsignal = map(abs(JOYSTICK_VERTICAL + JOYSTICK_HORIZONTAL), 0, JOYSTICK_VERTICAL_MAX + JOYSTICK_HORIZONTAL_MAX, 191,216); // forward right w/ button
            }
        }

        else if(JOYSTICK_VERTICAL < -100) { // Joystick is in the last two quadrants
            if(JOYSTICK_HORIZONTAL > 100){ // Joystick is in third quadrant
                IRsignal = map(JOYSTICK_VERTICAL + JOYSTICK_HORIZONTAL, 0, JOYSTICK_VERTICAL_MAX + JOYSTICK_HORIZONTAL_MAX, 217, 232); // backward left w/ button
            }

            else if(abs(JOYSTICK_HORIZONTAL) <= 100) { // Joystick is in negative y-axis
                IRsignal = map(JOYSTICK_VERTICAL, 101, JOYSTICK_VERTICAL_MAX, 233, 238); // backward middle w/ button
            }

            else if(JOYSTICK_HORIZONTAL < -100) { // Joystick is in fourth quadrant
                IRsignal = map(abs(JOYSTICK_VERTICAL + JOYSTICK_HORIZONTAL), 0, JOYSTICK_VERTICAL_MAX + JOYSTICK_HORIZONTAL_MAX, 239,254); // backward right w/ button
            }
        }
    }

    else { // Button is not pressed
        if(JOYSTICK_VERTICAL > 100) { // Joystick is in the first two quadrants
            if(JOYSTICK_HORIZONTAL > 100){ // Joystick is in second quadrant
                // The more the joystick tends to the extremities, the higher the value of the signal
                IRsignal = map(JOYSTICK_VERTICAL + JOYSTICK_HORIZONTAL, 0, JOYSTICK_VERTICAL_MAX + JOYSTICK_HORIZONTAL_MAX, 0, 54); // forward left w/o button
            }

            else if(abs(JOYSTICK_HORIZONTAL) <= 100) { // Joystick is in positive y-axis
                IRsignal = map(JOYSTICK_VERTICAL, 101, JOYSTICK_VERTICAL_MAX, 55, 64); // forward middle w/o button
            }

            else if(JOYSTICK_HORIZONTAL < -100) { // Joystick is in first quadrant
                IRsignal = map(abs(JOYSTICK_VERTICAL + JOYSTICK_HORIZONTAL), 0, JOYSTICK_VERTICAL_MAX + JOYSTICK_HORIZONTAL_MAX, 65,119); // forward right w/o button
            }
        }

        else if(JOYSTICK_VERTICAL < -100) { // Joystick is in the last two quadrants
            if(JOYSTICK_HORIZONTAL > 100){ // Joystick is in third quadrant
                IRsignal = map(JOYSTICK_VERTICAL + JOYSTICK_HORIZONTAL, 0, JOYSTICK_VERTICAL_MAX + JOYSTICK_HORIZONTAL_MAX, 120, 136); // backward left w/o button
            }

            else if(abs(JOYSTICK_HORIZONTAL) <= 100) { // Joystick is in negative y-axis
                IRsignal = map(JOYSTICK_VERTICAL, 101, JOYSTICK_VERTICAL_MAX, 137, 142); // backward middle w/o button
            }

            else if(JOYSTICK_HORIZONTAL < -100) { // Joystick is in fourth quadrant
                IRsignal = map(abs(JOYSTICK_VERTICAL + JOYSTICK_HORIZONTAL), 0, JOYSTICK_VERTICAL_MAX + JOYSTICK_HORIZONTAL_MAX, 143,159); // backward right w/o button
            }
        }
    }

    return(IRsignal);
}

void setup() {
    Serial.begin(9600); // Allow sending and receiving of data from NANO to Serial Monitor at 9600 baud rate

    // To know which program is running on NANO
    Serial.print("Start ");
    Serial.print(__FILE__);
    Serial.print(" from ");
    Serial.print(__DATE__);

    // Start the IR transmitter, enable feedback LED
    IrSender.begin(IR_TRANSMITTER_PIN, true);

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

    // Set LEDs to output
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    // Set Joystick pins to input
    pinMode(JOYSTICK_VERTICAL_PIN, INPUT);
    pinMode(JOYSTICK_HORIZONTAL_PIN, INPUT);
    pinMode(JOYSTICK_SWITCH_PIN, INPUT);

    // Initiate light show sequence which matches that of the STRATUS's
    Serial.println("Initiating start-up light show sequence...");
    startupLightShow();
}

void loop() {
    sCommand = mapJoystickVoltageToIRSignal();

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