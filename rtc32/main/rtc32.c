#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "esp32/rom/rtc.h"
#include "esp32/rom/crc.h"
#include "sys/time.h"

#define TAG "RTC"

void read_rtc(void *pvParameter)
{
    while (1) {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);
        printf("Current time: %04d-%02d-%02d %02d:%02d:%02d\n",
               timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
               timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    esp_err_t ret;
    struct tm timeinfo;
    time_t now;

    // Set default time to January 1st, 2023 at 00:00:00
    struct tm default_time = {
        .tm_year = 2023 - 1900,
        .tm_mon = 0, // January
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    time_t default_time_secs = mktime(&default_time);
    struct timeval tv = {
        .tv_sec = default_time_secs,
        .tv_usec = 0
    };
    printf("Setting default time to %lld\n", default_time_secs);
    settimeofday(&tv, NULL);

    // Initialize the RTC GPIO
    printf("Initializing RTC GPIO\n");
    ret = rtc_gpio_init(GPIO_NUM_26);
    if (ret != ESP_OK) {
        printf("Error initializing RTC GPIO: %d\n", ret);
        return;
    }
    ret = rtc_gpio_set_direction(GPIO_NUM_26, RTC_GPIO_MODE_INPUT_ONLY);
    if (ret != ESP_OK) {
        printf("Error setting RTC GPIO direction: %d\n", ret);
        return;
    }

    xTaskCreate(&read_rtc, "read_rtc", 2048, NULL, 5, NULL);
}
