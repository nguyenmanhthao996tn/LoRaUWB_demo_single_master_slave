#include "LED_Effects.h"

void LedEffect_ProgramStart(void)
{
  BoardLed.ledBlueBlink(2); // Blink 2 times
  delay(20);
  BoardLed.ledRedBlink(2); // Blink 2 times
  delay(20);
  BoardLed.ledBlueBlink(2); // Blink 2 times
  delay(20);
  BoardLed.ledRedBlink(2); // Blink 2 times
  delay(20);
}

void LedEffect_SlaveModeStart(void)
{
  BoardLed.ledRedBlink(3);
  BoardLed.ledRedOn();
  delay(500);
  BoardLed.ledRedOff();
}

void LedEffect_MasterModeStart(void)
{
  BoardLed.ledBlueBlink(3);
  BoardLed.ledBlueOn();
  delay(500);
  BoardLed.ledBlueOff();
}

void LedEffect_MasterMeasure(void)
{
  BoardLed.ledBlueOn();
  delay(1000);
  BoardLed.ledBlueOff();
}

void LedEffect_MasterMeasureFail(void)
{
  BoardLed.ledRedBlink(5);
  BoardLed.ledBlueOff();
}

void LedEffect_MasterMeasureSuccess(void)
{
  BoardLed.ledBlueBlink(5);
  BoardLed.ledBlueOff();
}
