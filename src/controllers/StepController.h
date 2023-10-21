#pragma once

#include <Arduino.h>
#include <esp_log.h>

#include "common.h"

#define STEP_TAG "Step"

void setupStep() {
    pinMode(PIN_STEP_BUTTON, INPUT);

    ledcSetup(CH_STEP_MOTOR, 1000, 8);
    ledcAttachPin(PIN_STEP_MOTOR, CH_STEP_MOTOR);
}

int lastStepButtonState = HIGH;
bool isWalking = false;

void checkStepBtnClicked() {
    auto stepBtnClick = digitalRead(PIN_STEP_BUTTON);
    bool changed = false;
    if ((stepBtnClick == HIGH) && (lastStepButtonState == LOW)) {
        changed = true;
    }
    if(changed) {
        isWalking = !isWalking;
        if (isWalking) {
            ESP_LOGD(STEP_TAG, "Walk");
            ledcWrite(CH_STEP_MOTOR, 255);
        } else {
            ESP_LOGD(STEP_TAG, "Stop walking");
            ledcWrite(CH_STEP_MOTOR, 0);
        }
    }

    lastStepButtonState = stepBtnClick;
}
