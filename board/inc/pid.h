#ifndef __PID_H
#define __PID_H

#include "stm32f4xx_conf.h"

void PID_init(void);
float PID_realize(float speed);

#endif

