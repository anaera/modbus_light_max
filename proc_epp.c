#include <xc.h>
#include "proc_epp.h"

/**
  Section: Data EEPROM Module APIs
 */

void DATAEE_WriteByte(uint8_t bAdd, uint8_t bData) {
    uint8_t GIEBitValue = 0;

    EEADRL = bAdd ; // Data Memory Address to write
    EEDATL = bData; // Data Memory Value to write
    EECON1bits.EEPGD = 0; // Point to DATA memory
    EECON1bits.CFGS = 0; // Deselect Configuration space
    EECON1bits.WREN = 1; // Enable writes

    //    GIEBitValue = INTCONbits.GIE;
    INTCONbits.GIE = 0; // Disable INTs
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1; // Set WR bit to begin write
    INTCONbits.GIE = 1; // enable INTs
    // Wait for write to complete
     EECON1bits.WREN = 0; // Disable writes   
    while (EECON1bits.WR) {
 }
}

uint8_t DATAEE_ReadByte(uint8_t bAdd) {
    EEADRL = (uint8_t) (bAdd & 0x0ff); // Data Memory Address to read
    EECON1bits.CFGS = 0; // Deselect Configuration space
    EECON1bits.EEPGD = 0; // Point to DATA memory
    EECON1bits.RD = 1; // EE Read
    NOP(); // NOPs may be required for latency at high frequencies
    NOP();

    return (EEDATL);
}
/**
 End of File
 */