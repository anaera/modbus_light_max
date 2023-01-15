
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef DATA_EPP_H
#define	DATA_EPP_H

#include <xc.h> // include processor files - each processor file is guarded.  

typedef struct {
    uint8_t pin1;
    uint8_t pin2;
} actOut_t;

typedef struct {
    uint8_t timeUp;
    uint8_t timeDn;
} timeUpDn_t;

typedef union {
    //   uint8_t _ee_data [256];

    struct {
        //Гдобальные параметры

        struct {
            uint8_t _aDev; //адрес устройства
            uint8_t _nSpeed; //номер скорости интерфейса
            uint8_t _rInp; //кол-во входных регистров (input)
            uint8_t _rOut; //кол-во выходных регистров (coil)
            uint8_t _bOut; //кол-во всех out регистров
            uint8_t quickTime; //время быстрого нажатия quickPress
            uint8_t longTime; //время долгого нажатия longPress
            uint8_t repitTime; //время до начала повторов repitPress

            uint8_t _tabSpeed[4]; //делитель скорости интерфеса
            uint8_t _tabGap[4]; //время защитного интервала
            uint8_t dummy[16];

            uint8_t _addrReg[8]; // таблица адресов регистров
            uint8_t _dataReg[8]; // начальнон значение бит
        };

        //набор флагов управления для inp и out  пинов
        uint8_t flagInvert[8];

        struct {

            union {
                uint8_t flagActPress[8]; //inp  (0 - (AI_REG-1)) flagActEnable
                uint8_t flagActTimer[8]; //out  ((AI_REG - AI_REG+AO_REG) - 1)

            };
        };

        struct {

            union {
                uint8_t flagPress[8]; //inp  (0 - (AI_REG-1))  flagTypeAction
                uint8_t flagNight[8]; //out  ((AI_REG - AI_REG+AO_REG) - 1)
            };
        };

        struct {
            //набор altAct для inp пинов и таймеров для output

            union {
                actOut_t altPin[64]; //inp (0 - (AI_REG-1))
                timeUpDn_t valTimer[64]; //out ((AI_REG - AI_REG+AO_REG) - 1)
            };
        };
    };
} ee_t;


#endif	/* DATA_EPP_H */

