#include <xc.h>
#include "const.h"

__eeprom ee_t ee = {
  ADDR_DEV, SPEED_NUM, INP_REG, OUT_REG, OUT_BUS, QL_TIME, LR_TIME, RS_TIME, //8
  BAUD_1920, BAUD_3840, BAUD_5760, BAUD_11520, GAP_1920, GAP_3840, GAP_5760, GAP_11520,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  //адреса регистров и их их начальные значения
  32, 33, 34, 35, 37, 38, 36, 39, // address 
  INP_MASK, INP_MASK, INP_MASK, INP_MASK, INP_MASK, INP_MASK, OUT_VAL, OUT_VAL, //value

  // инверсия входов/выходов
  NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, INVERT, INVERT, //flagInvert
  //input actPress - рабочие/нет входы; output actTimer - удаленная активация таймера 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, //  flagActPress/flagActTimer

  // input Press/Update output always/night
  0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, // flagPress/flagNight



  0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33, //init outPins&Timers
  0x34, 0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x37, //init outPins&Timers

  0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33, 0x34, //init outPins&Timers
  0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x37, 0x30, //init outPins&Timers

  0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33, //init outPins&Timers
  0x34, 0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x37, //init outPins&Timers

  0x40, 0x40, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, //init outPins&Timers
  0x44, 0x44, 0x45, 0x45, 0x46, 0x46, 0x47, 0x47, //init outPins&Timers

  0x40, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x44, //init outPins&Timers
  0x44, 0x45, 0x45, 0x46, 0x46, 0x47, 0x47, 0x40, //init outPins&Timers

  0x40, 0x40, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, //init outPins&Timers
  0x44, 0x44, 0x45, 0x45, 0x46, 0x46, 0x47, 0x47, //init outPins&Timers

  0x10, 0x08, 0x11, 0x09, 0x12, 0x0A, 0x13, 0x0B, //init outPins&Timers
  0x14, 0x0C, 0x15, 0x0D, 0x16, 0x0E, 0x17, 0x0F, //init outPins&Timers

  0x10, 0x08, 0x11, 0x09, 0x12, 0x0A, 0x13, 0x0B, //init outPins&Timers
  0x14, 0x0C, 0x15, 0x0D, 0x16, 0x0E, 0x17, 0x0F, //init outPins&Timers

};