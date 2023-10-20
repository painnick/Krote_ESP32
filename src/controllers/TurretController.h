#pragma once

#include <Arduino.h>
#include <esp_log.h>
#include "ESP32Servo.h"

#include "common.h"

Servo turretServo;

#define TURRET_TAG "Turret"

#define TURRET_CENTER_ANGLE 30
#define TURRET_MOVE_STEP 3

int targetTurretAngle = TURRET_CENTER_ANGLE;
int currentTurretAngle = TURRET_CENTER_ANGLE;
bool turretRotateForward = true;

void loopTurret() {
    int angleDiff = abs(targetTurretAngle - currentTurretAngle);
    if (angleDiff >= 15) {
        int lastAngle = currentTurretAngle;
        int step = (random() % 5);
        if (turretRotateForward) {
            currentTurretAngle += step;
        } else {
            currentTurretAngle -= step;
        }
        turretServo.write(currentTurretAngle);
        ESP_LOGD(TURRET_TAG, "Turret move %d -> %d, Target %d", lastAngle, currentTurretAngle, targetTurretAngle);

        delay(50);
    } else {
        int sleepSec = angleDiff + (random() % 5);
        for (int i = 0; i < sleepSec; i++) {
            delay(1000);
        }

        int lastTargetAngle = targetTurretAngle;
        int newTargetAngle = TURRET_CENTER_ANGLE + (random() % 9) * 10;
        turretRotateForward = (newTargetAngle > lastTargetAngle);
        targetTurretAngle = newTargetAngle;

        ESP_LOGD(TURRET_TAG, "Set target %d => %d", lastTargetAngle, newTargetAngle);
    }

/**
 * Sample
    currentTurretAngle = (currentTurretAngle + 5) % 60;
    turretServo.write(60 + currentTurretAngle);
    ESP_LOGD(TURRET_TAG, "Current %d", currentTurretAngle);
    if(currentTurretAngle == 0) {
        delay(3000);
    } else {
        delay(500);
    }
*/
}

void setupTurret() {
    turretServo.attach(PIN_TURRET_SERVO, 500, 2500);
    turretServo.write(currentTurretAngle);
}
