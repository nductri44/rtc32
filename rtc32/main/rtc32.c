#include <stdio.h>
#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "rtc.h"

void app_main()
{
    // initialize the RTC
    rtc_gpio_init(4); 

    // set the time to the current time
    int64_t current_time_us = esp_timer_get_time();
    rtc_clk_set(current_time_us);

    // get the current time from the RTC
    int64_t rtc_time_us = rtc_clk_get();
    printf("RTC time: %lld microseconds\n", rtc_time_us);
}
