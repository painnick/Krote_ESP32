#pragma once

#include <Arduino.h>
#include <esp_log.h>

#include "common.h"
#include "TurretController.h"

#define RADAR_TAG "Radar"

TaskHandle_t radarTaskHandle;

[[noreturn]] void taskRadar(__attribute__((unused)) void *params) {
    while (true) {
        ESP_LOGD(RADAR_TAG, "Radar");

        for (int i = 0; i < 20; i++) {
            delay(1000);
        }

        suspendTurret();

        delay(1000 * 3);

        int repeat = 1 + (random() % 3);
        for(int i = 0; i < repeat; i ++) {
            ledcWrite(CH_RADAR_MOTOR, 127);
            delay(1000 * 1);
            ledcWrite(CH_RADAR_MOTOR, 0);
            delay(1000 * 2);
        }

        resumeTurret();
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