#pragma once

#include <Arduino.h>
#include <esp_log.h>
#include "ESP32Servo.h"

#include "common.h"

Servo turretServo;

#define TURRET_TAG "Turret"

#define TURRET_CENTER_ANGLE 90

int targetTurretAngle = TURRET_CENTER_ANGLE;
int minTurretAngle = TURRET_CENTER_ANGLE - 45;
int maxTurretAngle = TURRET_CENTER_ANGLE + 45;

bool isTurretSuspended = false;
bool isTurretOn = true;
unsigned long turretWaitStart = 0;
unsigned long turretWaitMs = 1000;
bool isTurretForward = true;
bool isTurretDirectionChanged = false;

void loopTurret(unsigned long now) {
    if(!isTurretSuspended) {
        if (isTurretOn) {
            turretServo.write(targetTurretAngle);
            isTurretOn = false;
            turretWaitStart = now;
            if (isTurretDirectionChanged) {
                turretWaitMs = 1000 * 5;
                ESP_LOGD(TURRET_TAG, "Wait %d ms", turretWaitMs);
            } else {
                turretWaitMs = (1 + (random() % 3)) * 500;
            }
        } else {
            if (now - turretWaitStart > turretWaitMs) {
                int step = (1 + random() % 4) * 5;
                if (isTurretForward) {
                    targetTurretAngle += step;
                } else {
                    targetTurretAngle -= step;
                }
                if (targetTurretAngle > maxTurretAngle) {
                    targetTurretAngle = maxTurretAngle;
                    maxTurretAngle = 90 + (1 + (random() % 5)) * 9;
                    ESP_LOGD(TURRET_TAG, "New Max angle %d", maxTurretAngle);
                    isTurretForward = false;
                    isTurretDirectionChanged = true;
                } else if (targetTurretAngle < minTurretAngle) {
                    targetTurretAngle = minTurretAngle;
                    minTurretAngle = 90 - (1 + (random() % 5)) * 9;
                    ESP_LOGD(TURRET_TAG, "New Min angle %d", minTurretAngle);
                    isTurretForward = true;
                    isTurretDirectionChanged = true;
                } else {
                    isTurretDirectionChanged = false;
                }
                isTurretOn = true;
                ESP_LOGD(TURRET_TAG, "Target angle %d", targetTurretAngle);
            }
        }
    }
}

void setupTurret() {
    turretServo.attach(PIN_TURRET_SERVO, 500, 2500);
    turretServo.write(targetTurretAngle);
}

void suspendTurret() {
    isTurretSuspended = true;
}

void resumeTurret() {
    isTurretSuspended = false;
}