#include "timer.h"

#include "FreeRTOS.h"
#include "task.h"


extern I2C_HandleTypeDef hi2c2;

static void DisplayInit()
{
    OLED_ConfigDisplay(&hi2c2, 0x78);
    (void)OLED_InitDisplay();
    OLED_NewFrame();
    OLED_ShowString(0, 0, "TIMER INIT....");
    OLED_ShowFrame();
}

void TimerLogic(void *arg)
{
    (void)arg;

    DisplayInit();
    vTaskDelay(500);

    while (1)
    {
        
    }
}