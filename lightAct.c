/*
 * File:   newmain.c
 * Author: Papa
 *
 * Created on 12 ??????? 2022 ?., 21:31
 */


#include <xc.h>
//#include <stdint.h>
#include "const.h"
#include "timers.h"
extern __eeprom ee_t ee;
extern uint8_t timers[8 * TM_REG];
extern uint8_t regCnt, regOUT, regALL;
extern COMMON_DATA COMMON;
extern pin_t pin;
void startActTimers(uint8_t nPin);
void startLockTimers(uint8_t nPin);

uint8_t bitCnt, bitMask;
//Массив таймеров от 0 до 8*(кол-во out-регистров) - (16)
//Масси out-регистов от (ee.Inp) до (ee._rInp + ee._nTimers)
//-----------------------------------------------------------------------------

void actValue(uint8_t numPin) {
  uint8_t nByte, nMask;
  nByte = (uint8_t) (numPin >> 3);
  nMask = (uint8_t) (1 << (numPin & 07));

  if (COMMON.regALL[nByte] & nMask) { //если вкл, то выкл
    if (COMMON.regALL[nByte + regOUT] & nMask) { // ручной/автоматич
      COMMON.regALL[nByte] &= ~nMask; // ->0  //ручной
    } else {
      startLockTimers(numPin); //автомат
    }
  } else { // иначе включить
    if (COMMON.regALL[nByte + regOUT] & nMask) {
      COMMON.regALL[nByte] |= nMask; //->1 //ручной
    } else {
      pin.lock[nByte] &= (~nMask); // сбросить флаг блокировки
      startActTimers(numPin); //автомат
    }
  };
}
//-----------------------------------------------------------------------------

void actManual(uint8_t numPin) {
  uint8_t nByte, nMask;
  nByte = (uint8_t) (numPin >> 3);
  nMask = (uint8_t) (1 << (numPin & 07));

  if (COMMON.regALL[nByte] & nMask) { //если вкл, то длинное нажатие +/- р.упр.
    COMMON.regALL[nByte + regOUT] |= nMask; // + ручное управление
  } else {
    COMMON.regALL[nByte + regOUT] &= (~nMask); // - ручное управление}
  }
}
//-----------------------------------------------------------------------------

void actFlash(uint8_t numPin) {

#define _DAY_MASK 1
  uint8_t nByte, nMask;
  nByte = (uint8_t) (numPin >> 3);
  nMask = (uint8_t) (1 << (numPin & 07));

  if (COMMON.regALL[nByte + regOUT] & nMask) //ручное управление ?
  {
    return; //ДА!
  } else { //НЕТ!  автомат , далее
    if ((ee.flagNight[nByte] & nMask)) { //только когда темно - ночью
      if (COMMON.regALL[regALL + regOUT] & _DAY_MASK) //если только ночью,а сейчас день
      {
        return; //сейчас день
      }
    }
    //переустановили таймер и обновили regOut
    startActTimers(numPin);
  }
}
//-----------------------------------------------------------------------------
//Читаем из ROM номера портов для действия и идем выполнять

void doSetValue(pressHead_t numPin) {
  //номер первого действия на порту
  //  numPin = ee.altPin[numPin.numPin].pin1;
  actValue(ee.altPin[numPin.numPin].pin1);
  //номер второго действия на порту
  if (ee.altPin[numPin.numPin].pin1 == ee.altPin[numPin.numPin].pin2) return;
  actValue(ee.altPin[numPin.numPin].pin2);
}
//-----------------------------------------------------------------------------

void doSetManual(pressHead_t numPin) {
  //номер первого действия на порту
  //numPin = ee.altPin[numPin.numPin].pin1;
  actManual(ee.altPin[numPin.numPin].pin1);
  //номер второго действия на порту
  if (ee.altPin[numPin.numPin].pin1 == ee.altPin[numPin.numPin].pin2) return;
  actManual(ee.altPin[numPin.numPin].pin2);
}
//-----------------------------------------------------------------------------

void doSetFlash(pressHead_t numPin) {
  //номер первого действия на порту
  //numPin = ee.altPin[numPin.numPin].pin1;
  actFlash(ee.altPin[numPin.numPin].pin1);
  //номер второго действия на порту
  if (ee.altPin[numPin.numPin].pin1 == ee.altPin[numPin.numPin].pin2) return;
  actFlash(ee.altPin[numPin.numPin].pin2);
}
//-----------------------------------------------------------------------------

enum {
  _EMPTY = 0,
  _QUICK = 1,
  _LONG = 2,
  _REPIT = 3,
};

void doAction(pressHead_t numPin) {
  uint8_t nByte, nMask;

  //  uint8_t message;
  //message = (uint8_t) numPin.head;

  nByte = (uint8_t) (numPin.numPin >> 3);
  nMask = (uint8_t) (1 << (numPin. numPin & 0x07));

  if (ee.flagPress[nByte] & nMask) {
    if (COMMON.regALL[nByte] & nMask) {
      if (numPin.state == _QUICK) {
        doSetValue(numPin);
        return;
      }

      if (numPin.state == _LONG) {
        doSetManual(numPin);
        return;
      }
    }
  } else {
    if (numPin.state == _EMPTY)
      doSetFlash(numPin);
  }
}
