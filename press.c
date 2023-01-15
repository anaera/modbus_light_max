/*
 * File:   press.c
 * Author: 79166
 *
 * Created on 20 декабря 2022 г., 14:07
 */
#include <xc.h>
#include "const.h"
enum {
  _EMPTY = 0,
  _QUICK = 1,
  _LONG = 2,
  _REPIT = 3,
};

enum {
  _ACT = 0,
  _LOCK = 1,
};

enum {
  _FREE = 0,
  _BUSY = 3,
};

typedef struct {
  pressHead_t headTimer;
  uint8_t valTimer;
} queue_t;

uint8_t lastElement = 0;


pin_t pin;
queue_t timeQueue[T_MAX];

extern __eeprom ee_t ee;
extern uint8_t regCnt; //текущий регистр
extern COMMON_DATA COMMON;
//------------------------------------------------------------------------------
void doAction(pressHead_t numPin);

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void stopActTimers(uint8_t cnt) { //обрабатываем элемент таймера с номером cnt
  uint8_t nPin, nMask, nByte;
  timeQueue[cnt].headTimer.state = _FREE; //освободить элемент таймера
  nPin = timeQueue[cnt].headTimer.numPin; //получить номер освобождаемого таймера
  nMask = (uint8_t) (1 << (nPin & 0x07)); //маска бита
  nByte = (uint8_t) (nPin >> 3)&0x07; //номер байта
  if (pin.lock[nByte] & nMask) { //какой таймер (Lock/Act)
    pin.lock[nByte] &= ~nMask; //Lock -> снять блокировку
  } else { // Act -> гасим Pin -> 0
    COMMON.regALL[nByte] &= ~nMask; // ->0     
  }
}
//------------------------------------------------------------------------------

uint8_t findTimersEntry(uint8_t nPin) {
  uint8_t nMask;
  uint8_t nByte;
  uint8_t nQueue;
  nMask = (uint8_t) (1 << (nPin & 0x07));
  nByte = (uint8_t) (nPin >> 3)&0x07;
  for (uint8_t cnt = 0; cnt < lastElement; cnt++) { // ищем есть ли его таймер
    if ((timeQueue[cnt].headTimer.numPin) == nPin) { // нашли
      return cnt; //будет переустановка таймера
      break;
    };
  };
  for (uint8_t cnt = 0; cnt < lastElement; cnt++) { //да, ищем свободный элемент
    if ((timeQueue[cnt].headTimer.state) == _FREE) { // нашли
      return cnt; //будет установка нового таймера
      break;
    };
  };
  //нет активного таймера или свободного - выделяем
  if (lastElement < T_MAX) {
    lastElement++; //добавляем новый в список
    return (lastElement - 1); //новая установка таймера
  } else {
    return 0xFF; //если свободных нет, про остальное забывам
  }
}
//------------------------------------------------------------------------------

void startLockTimers(uint8_t nPin) {
  uint8_t nMask;
  uint8_t nByte;
  uint8_t nQueue;
  nQueue = findTimersEntry(nPin);
  if (nQueue == 0xFF)return;

  nMask = (uint8_t) (1 << (nPin & 0x07));
  nByte = (uint8_t) (nPin >> 3)&0x07;
  pin.lock[nByte] = nMask | pin.lock[nByte]; // флаг блокировки
  COMMON.regALL[nByte] &= (~nMask); // ->0
  timeQueue[nQueue].valTimer = ee.valTimer[nPin].timeDn; // установили lockTime
  timeQueue[nQueue].headTimer.numPin = nPin;
  timeQueue[nQueue].headTimer.state = _BUSY;
  return;
}
//------------------------------------------------------------------------------

void startActTimers(uint8_t nPin) {
  uint8_t nMask;
  uint8_t nByte;
  uint8_t nQueue;
  nQueue = findTimersEntry(nPin);
  if (nQueue == 0xFF)return;

  nMask = (uint8_t) (1 << (nPin & 0x07));
  nByte = (uint8_t) (nPin >> 3)&0x07;
  if (pin.lock[nByte] == nMask) return; // в режиме блокировки
  COMMON.regALL[nByte] |= nMask; //->1
  timeQueue[nQueue].valTimer = ee.valTimer[nPin].timeUp; // установили actTime
  timeQueue[nQueue].headTimer.numPin = nPin;
  timeQueue[nQueue].headTimer.state = _BUSY;
  return;
}
//------------------------------------------------------------------------------

void dispatchTimers(void) {

  for (uint8_t cnt = 0; cnt < lastElement; cnt++) {
    if (timeQueue[cnt].headTimer.state) {//таймер активен
      //      nPin = timeQueue[cnt].headTimer.numPin;
      if ((timeQueue[cnt].headTimer.numPin >> 3) >= ee._rInp) {
        if ((timeQueue[cnt].valTimer) > 0) {
          timeQueue[cnt].valTimer--;
          if (timeQueue[cnt].valTimer == 0) { //таймер обнулился. Обработка.
            stopActTimers(cnt);
          };
        };
      };
    };
  };
  return;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void dispatchPress(void) {

  uint8_t nMask;
  uint8_t nByte;
  uint8_t nPin;

  for (uint8_t cnt = 0; cnt < lastElement; cnt++) { //перебираем элементы
    if (timeQueue[cnt].headTimer.state) {//таймер активен ?

      if ((timeQueue[cnt].headTimer.numPin >> 3) < ee._rInp) { // input Pin ?
        if ((timeQueue[cnt].valTimer) > 0) { //да
          timeQueue[cnt].valTimer--;
          if (timeQueue[cnt].valTimer == 0) { //таймер обнулился. Обработка.
            //номер пина  -> номер регистра + маска бита
            nPin = timeQueue[cnt].headTimer.numPin;
            nMask = (uint8_t) (1 << (nPin & 0x07));
            nByte = (uint8_t) (nPin >> 3)&0x07;
            //смотрим какой интервал истек
            if (timeQueue[cnt].headTimer.state == _REPIT) { //истекло время до rPress
              //конец отсчетам - отрабатываем repitPress, поднимаем флаг покоя (rest)
              timeQueue[cnt].headTimer.state = _REPIT;
              doAction((pressHead_t) timeQueue[cnt].headTimer.head);
              timeQueue[cnt].headTimer.state = _EMPTY; //освобождаем таймер
              pin.active[nByte] = pin.active[nByte] & (~nMask); //сбросить флаг активности
              //COMMON.regALL[6] &= (~0x40); //->x0xxxxxx
              continue;
            }
            if (timeQueue[cnt].headTimer.state == _LONG) {//истекдо время l_Press, установим r_Time
              timeQueue[cnt].valTimer = ee.repitTime;
              timeQueue[cnt].headTimer.state = _REPIT;
              //COMMON.regALL[6] &= (~0x20); //->xx0xxxxx
              continue;
            }
            if (timeQueue[cnt].headTimer.state == _QUICK) {//истекдо время q_Press, установим l_Time
              timeQueue[cnt].valTimer = ee.longTime; //следующее время
              timeQueue[cnt].headTimer.state = _LONG;
              //COMMON.regALL[6] &= (~0x10); //->xxx0xxxx 
              continue;
            }
          };
        };
      };
    };
  };
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void updatePress(uint8_t action) {
  uint8_t nBit;
  uint8_t nQueue;
  uint8_t nPin;
  uint8_t cnt;

  uint8_t nMask = 1; //regCnt глобален -> nMask глобален ?

  for (nBit = 0; nBit < 8; nBit++) { //просматриваем последовательно все биты

    if (nMask & action) {//бит изменился
      nPin = nBit + (uint8_t) (regCnt << 3);
      if (pin.active[regCnt] & nMask) { // идет счет длины нажатия? 
        for (cnt = 0; cnt < lastElement; cnt++) { //да, ищем его таймер
          if ((timeQueue[cnt].headTimer.numPin) == nPin) { //нашли или НЕ ! нашли
            nQueue = cnt;
            if (timeQueue[cnt].headTimer.state == _REPIT) {// состояние repitPRESS          
              doAction((pressHead_t) timeQueue[cnt].headTimer.head);
              goto setTimer;
            }
            if (timeQueue[cnt].headTimer.state == _LONG) {// состояние longPRESS
              doAction((pressHead_t) timeQueue[cnt].headTimer.head);
              goto setTimer;
            }
            if (timeQueue[cnt].headTimer.state == _QUICK) {// состояние quickPRESS
              doAction((pressHead_t) timeQueue[cnt].headTimer.head);
              goto setTimer;
            }
          }
          NOP();
        }
      }
      //счет не идет, или не нашли ! -> создаем таймер
      for (uint8_t cnt = 0; cnt < lastElement; cnt++) { //перебираем элементы
        if (!(timeQueue[cnt].headTimer.state)) { //элемент свободен?
          nQueue = cnt;
          goto setTimer; //да 
        }
      };
      //нет свободных
      if (lastElement < T_MAX) {
        lastElement++; //добавляем новый в список
        nQueue = lastElement - 1;
      } else {
        return; //если свободных нет, про остальное забывам
      }
setTimer: // заполняем поля - начало отсчета
      timeQueue[nQueue].headTimer.numPin = nPin; //занимаем
      timeQueue[nQueue].headTimer.state = _QUICK; //устанавливаем флаг текущего состояния      
      timeQueue[nQueue].valTimer = ee.quickTime; //начало отсчета устанавливаем время q_Time
      pin.active[regCnt] = nMask | pin.active[regCnt]; // установить флага активности
      doAction((pressHead_t)nPin);
      //COMMON.regALL[6] |= 0xF0; //->1111xxxx 
    }
    nMask = (uint8_t) (nMask << 1);
  }
}

/*
            //нужно будет для отосылки предидущее сотояние пина
            if (COMMON.regINP[regCnt] & nMask) // состояние пина 
            {
              nPin = nPin & 0x7F; // 1 отсылать будем старое 0 и потом новое 1
            } else {
              nPin = nPin | 0x80; //0 отсылать будем старое 1 и потом новое 0
            }
 */