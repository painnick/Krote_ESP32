#pragma once

#include <Arduino.h>
#include <esp_log.h>

#include "common.h"

#define RADAR_TAG "Radar"

TaskHandle_t radarTaskHandle;

[[noreturn]] void taskRadar(__attribute__((unused)) void *params) {
    while (true) {
        ESP_LOGD(RADAR_TAG, "Radar");

        for (int i = 0; i < 20; i++) {
            delay(1000);
        }
        ledcWrite(CH_RADAR_MOTOR, 47);
        delay(1000 * 1);
        ledcWrite(CH_RADAR_MOTOR, 0);
    }
}

void setupRadar() {
    ledcSetup(CH_RADAR_MOTOR, 1000, 8);
    ledcAttachPin(PIN_RADAR_MOTOR, CH_RADAR_MOTOR);

    xTaskCreate(
            taskRadar,
            "Radar",
            10000,
            nullptr,
            1,
            &radarTaskHandle);

}

void suspendRadar() {
    vTaskSuspend(radarTaskHandle);
}

void resumeRadar() {
    vTaskResume(radarTaskHandle);
}