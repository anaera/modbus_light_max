/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef CONST_H
#define	CONST_H

#include <xc.h>
#include <stdint.h>
#include "data_epp.h"

#define _XTAL_FREQ      16000000L
#define FOSC_UART        16000000L

#define INP_MASK 0xFF
#define OUT_VAL 0x00
#define INVERT  0xFF
#define NORMAL  0x00

#define QL_TIME 70   //быстрое нажатие
#define LR_TIME 80  //длинное нажатие
#define RS_TIME 50   //начало повторов

#define T_MAX 32 //длина очереди таймеров

#define CHIP_1840

#define _TMR1H 0xF8
#define _TMR1L 0x30

//Константы конфигурации устройства

#define ADDR_DEV   18   //Сетевой адрес modbus устройства
#define SPEED_NUM   0   //Рабочая скорость modbus устройства

//Константы конвигурации скорости UART
//задержка - 4 периода передачи одного байта - 10бит
//TMR0 - 64us
#define BAUD_1920   207
#define GAP_1920    32       //2,084ms = 0.52*4 (0.52 - 10bit) 2084/64=32

#define BAUD_3840   103
#define GAP_3840    16       //1,04ms = 0.26*4   

#define BAUD_5760   68
#define GAP_5760    10       //0,7ms = 0.173*4

#define BAUD_11520   34     
#define GAP_11520    6      //0.35ms = 0.086*4


//Интервалы заддаются в тиках таймера TO равного 1-ой миллисекунде
//#define GAP_TIME 4           // 4 МС -задержка от 4 до 5 мс(9600)
#define STEP_TIME 10          // 10МС
#define WAIT_TIME 100       // 1C

//Конфигурация пространства регистров устройства
//Для 8-битных контролеров для адресации целесообразно взять один байт,
//что позволит адресовать 512 аналоговых регистров, ALL_REG=AI_REG+AO_REG<512.
//Будем считать что этого достаточно.

//Пусть часть аналоговых регистров перекрываются с цифровыми.
//DI0,DI1,DI2,DI3,DI4,DI5,DI6,DI7 | DI8,DI9,DI10,DI11,DI12,DI13,DI14,DI15 | 
//--------------AI0--------------- | ------------------AI1---------------- | AI2 | AI3..

//Битовых регистров (DI и DO)в устройстве до 256 штук (максимально) с адресами от 0 до 255).
//Они перекрывают байтовые регистры (AI и AO) с адресами 0 до 63 (максимально),
//то есть к ним можно получить доступ через регистры AI и AO, соотвественно.
//Регистров (АI + АO)в устройстве так же может быть до 256 штук (максимально).
//  "++++++++ ++++++++ ++++++++ 
//  "-------- -------- -------- -------- -------- -------- -------- --------"
//Регистры (АI и АO) размером один байт. 

// В каждой конкретной реализации количество регистров задается соответственно 
// константами DI_REG, DO_REG, AI_REG, AO_REG
// Количество одновременно передаваемых регистров в пакете модбас в одной
// посылке ограничим AREG_CNT для аналоговых регистров и DREG_CNT для цифровых.
//
//В пакете MODBUS может быть 4 аналоговых регистра (8байт) или 8 цифровых (1байт) 

//Базовые константы конфигурации пакета протокола устройства
#define AREG_CNT  4  /*!< Количество аналоговых регистров в пакете (ограничим -4) */
#define DREG_CNT  8  /*!< Количество цифровых регистров в пакете (ограничим -8) */

//Рабочие константы пакета  МОДБАС
#define CTL_PACK_SIZE   9 /*!< кол-во байт пакета без данных(см.presetMultipleRegisters16) */
#define ERR_PACK_SIZE   5 /*!< Количество байт пакета ошибки Modbus */
#define MAX_PACK_SIZE CTL_PACK_SIZE+2*AREG_CNT /*!< Максимальный размер в байтах пакета Modbus */

//Конфигурация регистров конкретного устройства
#define INP_REG 6   //входные физические регистры
#define OUT_REG 2   //выходные физические регистры

#define INT_REG 1   //внутренний регистр общего назначения
#define EPP_REG 2   //регистр адреса и данных для записи в EPP
#define MAN_REG OUT_REG //регистр ручного режима auto/manual

#define ALL_REG INP_REG+OUT_REG // всего физических регистров


#define WRK_REG MAN_REG+INT_REG+EPP_REG //все внутренние регистры (записи)

#define INP_BUS INP_REG //все входные регистры (чтения)
#define OUT_BUS OUT_REG+ WRK_REG //все выходные регистры (записи)
#define ALL_BUS INP_BUS+OUT_BUS //все регистры устройства

#define INP_BIT 8*INP_BUS
#define OUT_BIT 8*OUT_BUS

//#define AI_REG  3 /*!< количество байт регистров (Analog Input Registers) */
//#define AO_REG  1 /*!< количество байт регистров (Analog Output Holding Registers) */
//#define DI_REG  8*AI_REG /*!< количество битовых регистров (Discrete Input Contacts) */
//#define DO_REG  8*AO_REG /*!< количество битовых регистров (Discrete Output Coils) */
//#define ALL_REG  AO_REG+AI_REG /*!< Общее количество байт области регистров устройства */
#define TM_REG OUT_REG


#ifdef CHIP_1840
#define TRIS_TxE    TRISAbits.TRISA0
#define TRIS_SCL    TRISAbits.TRISA1
#define TRIS_SDA    TRISAbits.TRISA2
#define TRIS_INT    TRISAbits.TRISA3
#define TRIS_TxD    TRISAbits.TRISA4
#define TRIS_RxD    TRISAbits.TRISA5
#define PORT_TxE    PORTAbits.RA0
#define PORT_SCL    PORTAbits.RA1
#define PORT_SDA    PORTAbits.RA2
#define PORT_INT    PORTAbits.RA3
#define PORT_TxD    PORTAbits.RA4
#define PORT_RxD    PORTAbits.RA5
#endif


#define SND    1
#define RCV    0

typedef struct {
    uint8_t cnt0;
    uint8_t cnt1;
} core_data_t;

typedef struct {
    uint8_t addr;
    uint8_t data;
} core_i2c_t;

typedef union {
    unsigned char registrTable[ALL_BUS];
    unsigned char regALL[ALL_BUS];

    struct {
        unsigned char regINP[INP_BUS];
        unsigned char regOUT[OUT_BUS];
    };
} COMMON_DATA;

typedef union {

    struct {
        uint8_t slaveId_0;
        uint8_t codeFun_1;
        uint8_t hiAddr_2;
        uint8_t loAddr_3;
        uint8_t hiNum_4;
        uint8_t loNum_5;
        uint8_t hiCrc_6;
        uint8_t loCrc_7;
    };
} REQUEST_PACK;

typedef union {
    uint8_t head;

    struct {
        uint8_t numPin : 6;
        uint8_t state : 2;

    };
} pressHead_t;
typedef union {
  uint8_t active[ALL_REG]; // repitPress for input
  uint8_t lock[ALL_REG];
} //lock for output
pin_t;

#endif

