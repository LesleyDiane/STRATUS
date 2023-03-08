#include <Arduino.h>

// Infrared transmitter pin
#define IR_TRANSMITTER_PIN             9 // PWM
#define IR_TRANSMITTER_PIN_STRING     "9"

#define NO_LED_FEEDBACK_CODE // saves 566 bytes program space

// Set-up the data to be sent
uint16_t sAddress = 0xE; // Address used by remote is 0xE
uint8_t sCommand = 0x00; // The hex code will be stored here

#include <IRremote.hpp>

// delay must be greater than 5 ms, otherwise the receiver sees it as one long signal
#define DELAY_AFTER_SEND 10

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
}

void loop() {

    // Iterate the hex code from 0x00 to 0xFF
    for (sCommand; sCommand <= 0xFF; sCommand++) {
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
}