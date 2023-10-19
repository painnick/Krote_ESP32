#pragma once

#include <Arduino.h>
#include <esp_log.h>
#include "ESP32Servo.h"

#include "common.h"

Servo turretServo;

#define TURRET_TAG "Turret"

TaskHandle_t turretTaskHandle;

[[noreturn]] void taskTurret(__attribute__((unused)) void *params) {
    while (true) {
        ESP_LOGD(TURRET_TAG, "Turret move");

        turretServo.write(random() % 30);

        for (int i = 0; i < 10; i++) {
            delay(1000);

        }
    }
}

void setupTurret() {
    turretServo.attach(PIN_TURRET_SERVO, 500, 2500);
    turretServo.write(0);

    xTaskCreate(
            taskTurret,
            "Turret",
            10000,
            nullptr,
            1,
            &turretTaskHandle);
}
