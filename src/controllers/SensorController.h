#pragma once

#include <Arduino.h>
#include <esp_log.h>

#include "common.h"

#define SENSOR_TAG "Sensor"

TaskHandle_t sensorTaskHandle;

[[noreturn]] void taskSensor(__attribute__((unused)) void *params) {
    while (true) {
        ESP_LOGD(SENSOR_TAG, "Sensor");

        ledcWrite(CH_SENSOR_LIGHT, LIGHT_BASE);
        delay(150);
        ledcWrite(CH_SENSOR_LIGHT, LIGHT_BASE + LIGHT_STEP);
        delay(150);
        ledcWrite(CH_SENSOR_LIGHT, LIGHT_MAX);
        delay(300);
        ledcWrite(CH_SENSOR_LIGHT, LIGHT_BASE + LIGHT_STEP);
        delay(150);
        ledcWrite(CH_SENSOR_LIGHT, LIGHT_BASE);
        delay(150);
        ledcWrite(CH_SENSOR_LIGHT, 0);

        int delaySec = random(5, 10);
        for (int i = 0; i < delaySec; i++) {
            delay(1000);
        }
    }
}

void setupSensor() {
    ledcSetup(CH_SENSOR_LIGHT, 1000, 8);
    ledcAttachPin(PIN_SENSOR_LIGHT, CH_SENSOR_LIGHT);

    xTaskCreate(
            taskSensor,
            "Sensor",
            10000,
            nullptr,
            1,
            &sensorTaskHandle);
}

void suspendSensor() {
    vTaskSuspend(sensorTaskHandle);
}

void resumeSensor() {
    vTaskResume(sensorTaskHandle);
}
