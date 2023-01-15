/*
 * File:   main.c
 * Author: 1
 *
 * Created on 11 декабря 2022 г., 17:21
 */
#include <xc.h>
#include "const.h"
#include "timers.h"
void initCore(void);
void initDrive(void);
void initModbus(void);
void initI2Cbus(void);
void initINT(void);
void engineModbus(void);
void I2C_Close(void);
void startLoop(void);
void dispatchTimers(void);
void doAction(pressHead_t numPin);
void dispatchPress(void);
void updatePress(uint8_t ValAct);

void stopActTimers(uint8_t nPin);
void startActTimers(uint8_t nPin);
void startLockTimers(uint8_t nPin);
void dispatchTimers(void);

extern void (*loopFunc)(void);
extern void (*loopStep)(void);

extern __eeprom ee_t ee;
extern uint8_t timers[8 * TM_REG];
extern COMMON_DATA COMMON;
extern uint8_t regCnt;

enum {
  _EMPTY = 0,
  _QUICK = 1,
  _LONG = 2,
  _REPIT = 3,
};

void main(void) {
  NOP();
  NOP();
  initCore();
  initDrive();
  initModbus();
  initI2Cbus();
  NOP();
 /* uint8_t valAct;
  valAct = 0x11;
  regCnt = 1;
  COMMON.regINP[0] = 0xf0;
  updatePress(valAct); //2,2,2,2
  dispatchPress(); //1,1,1,1
  dispatchPress(); //1,1,1,1
  dispatchPress(); //1,1,1,1
  dispatchPress(); //1,1,1,1  

  regCnt = 1;
  uint8_t valAct;
  pressHead_t valueAct;
  valueAct.numPin = 3;
  valueAct.state = _QUICK;
  doAction(valueAct);
  dispatchTimers();
  valueAct.numPin = 3;
  valueAct.state = _LONG;
  doAction(valueAct);
  dispatchTimers();
  valueAct.state = _EMPTY;
  valueAct.numPin = 11;
  doAction(valueAct);
  dispatchTimers();
  valueAct.state = _EMPTY;
  valueAct.head = 17;
  doAction(valueAct);
  NOP();
  valAct = 0xFF;
  COMMON.regINP[0] = 0;
  updatePress(valAct); //2,2,2,2
  dispatchPress(); //1,1,1,1
  dispatchPress(); //1,1,1,1

  COMMON.regINP[0] = 1;
  valAct = 1;
  updatePress(valAct); //1,1,1,2
  dispatchPress(); //0,0,0,1 ->3,3,3,1
  dispatchPress();
  NOP();
  COMMON.regINP[0] = 3;
  valAct = 2;
  updatePress(valAct);
  dispatchPress();
  dispatchPress();
  NOP();
  dispatchPress();
   */
  initINT();

  NOP();
  loopFunc = startLoop;
  //PORT_TxE = 1;
  while (1) {
    if (TIMERS.flagTimers.SECFLG == 1) {
      TIMERS.flagTimers.SECFLG = 0;
      dispatchTimers();
    }
    if (TIMERS.flagTimers.STPFLG == 1) {
      TIMERS.flagTimers.STPFLG = 0;
      dispatchPress();

    }
    engineModbus();
    NOP();
    NOP();
    di();
    loopFunc();
    ei();
    NOP();
    NOP();
    CLRWDT();
  };
  I2C_Close();

};
