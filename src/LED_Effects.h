#ifndef __LED_EFFECT_H__
#define __LED_EFFECT_H__

#include <Arduino.h>
#include "LoRaUWB_LED.h"

void LedEffect_ProgramStart(void);
void LedEffect_SlaveModeStart(void);
void LedEffect_MasterModeStart(void);
void LedEffect_MasterMeasure(void);
void LedEffect_MasterMeasureFail(void);
void LedEffect_MasterMeasureSuccess(void);

#endif /* __LED_EFFECT_H__ */
