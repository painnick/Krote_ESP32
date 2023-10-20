#pragma once

#include <Arduino.h>
#include <esp_log.h>
#include "ESP32Servo.h"

#include "common.h"

Servo turretServo;

#define TURRET_TAG "Turret"

#define TURRET_CENTER_ANGLE 90

int targetTurretAngle = TURRET_CENTER_ANGLE;
int currentTurretAngle = TURRET_CENTER_ANGLE;
bool turretRotateForward = true;

unsigned long lastTurretCheckTime = 0;
boolean isTurretOn = true;
int turretWaitMs = 1000 * 10;

void loopTurret(unsigned long now) {
    if (isTurretOn) {
        if (now - lastTurretCheckTime > 50) {
            int angleDiff = abs(targetTurretAngle - currentTurretAngle);
            if (angleDiff >= 15) {
                int lastAngle = currentTurretAngle;
                int step = 2 + (random() % 6);
                if (turretRotateForward) {
                    currentTurretAngle += step;
                } else {
                    currentTurretAngle -= step;
                }
                turretServo.write(currentTurretAngle);
                ESP_LOGD(TURRET_TAG, "Turret move %d -> %d, Target %d", lastAngle, currentTurretAngle,
                         targetTurretAngle);
            } else {
                isTurretOn = false;
                turretWaitMs = 1000 * (angleDiff / 3 + (random() % 5));
                ESP_LOGD(TURRET_TAG, "Wait %d ms", turretWaitMs);
            }
            lastTurretCheckTime = now;
        }
    } else {
        int lastTargetAngle = targetTurretAngle;
        if (now - lastTurretCheckTime > turretWaitMs) {
            int newTargetAngle = TURRET_CENTER_ANGLE - 45 + (random() % 10) * 10;
            turretRotateForward = (newTargetAngle > lastTargetAngle);
            targetTurretAngle = newTargetAngle;

            ESP_LOGD(TURRET_TAG, "Set target %d => %d", lastTargetAngle, newTargetAngle);

            isTurretOn = true;
            lastTurretCheckTime = now;
        }
    }
}

void setupTurret() {
    turretServo.attach(PIN_TURRET_SERVO, 500, 2500);
    turretServo.write(currentTurretAngle);
}
