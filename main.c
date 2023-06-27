/*
 * Copyright (C) 2023 Andrea Sepielli
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file LICENSE in the top level directory for more details.
 */

/**
 * @{
 *
 * @file
 * @brief       Smart-Soil-Moisture-Monitor
 *
 * @author     Andrea Sepielli <andreasepielli97@gmail.com>
 *
 * @}
 */

#include <math.h>
#include "periph/gpio.h"
#include "periph/adc.h"
#include "analog_util.h"
#include "xtimer.h"
// #include "ds18.h"
#include "dht.h"
#define SOIL_MOISTURE_PIN ADC_LINE(0)
#define TEMPERATURE_PIN GPIO13
#define ADC_RES ADC_RES_12BIT
#define BUZZER_PIN GPIO23
#define RELAY_PIN GPIO12
#define SOIL_MOISTURE_THRESHOLD 24
#define TEMPERATURE_THRESHOLD 1000

int soil_moisture_value = 0;
int soil_moisture_percentage = 0;
dht_params_t params = {
    .pin = TEMPERATURE_PIN,
    .type = DHT11,
    .in_mode = GPIO_IN};
dht_t dev;
int16_t temperature;
int16_t humidity;

int main(void)
{
    gpio_init(BUZZER_PIN, GPIO_OUT);
    if (dht_init(&dev, &params) != DHT_OK)
    {
        printf("Initialization of DHT failed\n");
        return 1;
    }
    else
    {
        printf("Initialization of DHT success\n");
    }

    if (adc_init(ADC_LINE(0)) < 0)
    {
        printf("Initialization of ADC Moisture sensor failed\n");
        return 1;
    }
    else
    {
        printf("Initialization of ADC Moisture sensor success\n");
    }

    while (1)
    {
        // Read soil moisture sensor
        soil_moisture_value = adc_sample(SOIL_MOISTURE_PIN, ADC_RES);
        soil_moisture_percentage = adc_util_map(soil_moisture_value, ADC_RES, 100, 0);
        printf("Soil moisture percentage: %d\n", soil_moisture_percentage);

        // Read temperature and humidity sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK)
        {
            printf("Temperature: %.1f\n", (float)temperature / 10);
            printf("Humidity: %.1f\n", (float)humidity / 10);
        }

        // Activate buzzer cycle
        // gpio_set(BUZZER_PIN);
        // xtimer_usleep(100000);
        // gpio_clear(BUZZER_PIN);
        // xtimer_sleep(5);

        xtimer_sleep(1);
    }

    return 0;
}