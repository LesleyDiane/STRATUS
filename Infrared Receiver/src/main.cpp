#include <Arduino.h>

// Infrared Receiver pin
#define IR_RECEIVER_PIN               A5 // IN
#define IR_RECEIVER_PIN_STRING       "A5"

#define NEC_protocol // Use only NEC protocol

#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes per program space

#define sAddress 0xE // Address of remote is 0xE

#include <IRremote.hpp>

#define DELAY_AFTER_RECEIVE 10 // the delay after IR signal is received and decoded

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
}

void loop() {
    // Check if received signal is available and if yes, try to decode and print it.
    if(IrReceiver.decode()) {
        IrReceiver.printIRResultShort(&Serial);

        delay(DELAY_AFTER_RECEIVE);
    }

    // Enable receiving of next signal
    IrReceiver.resume();
}