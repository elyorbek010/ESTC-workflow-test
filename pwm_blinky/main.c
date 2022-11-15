#include "modules/logs.h"
#include "modules/pwm.h"
#include "modules/button.h"

#define DEVICE_ID 7201
#define ID_HUE 4          // 360 * 0.01 = 3.6 ~ 4
#define CYCLE_PERIOD 1000 // T=1000us = 1ms,  Freq = 1/T = 1kHz

uint8_t mode = 1;
extern uint32_t t_on;
extern uint32_t duty_cycle;
extern bool btn_pressed;
extern bool btn_double_click;

int main(void)
{
  logs_init();
  bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS);
  timer_init();
  gpiote_init();

  while (true)
  {
    send_log(); // Periodically send logs

    if (mode > 4)
    {
      mode = 1;
    }

    if (btn_double_click)
    {
      mode++;
      btn_double_click = false;
      NRF_LOG_INFO("Button is double clicked: mode = %u", mode);
    }

    switch (mode)
    {
    case 1:
      no_modif();
      break;
    case 2:
      hue_modif();
      break;
    case 3:
      saturation_modif();
      break;
    case 4:
      value_modif();
      break;
    default:
      break;
    }

    if (btn_pressed)
    {
      NRF_LOG_INFO("Button is pressed \nDuty cycle: %lu%%", duty_cycle);
      fluctuate_duty_cycle(); // change duty cycle 0-100-0 percent
    }

    pwm_blink(t_on, CYCLE_PERIOD);
  }
}
