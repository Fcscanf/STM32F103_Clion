//
// Created by fcant on 2025/9/5 ÐÇÆÚÎå.
//

#ifndef STM32F103_CLION_WDG_H
#define STM32F103_CLION_WDG_H

#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_wwdg.h"

void WWDG_Init(void);
void WWDG_Refresh(void);

#endif //STM32F103_CLION_WDG_H