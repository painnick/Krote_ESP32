#pragma once

#include <Arduino.h>
#include <esp_log.h>

#include "common.h"

#define SEARCH_LIGHT_TAG "Search"

void setupSearchLight() {
    pinMode(PIN_SEARCH_LIGHT_BUTTON, INPUT);

    ledcSetup(CH_SEARCH_LIGHT, 1000, 8);
    ledcAttachPin(PIN_SEARCH_LIGHT, CH_SEARCH_LIGHT);
}

int lastSearchLightButtonState = HIGH;
int searchLightBright = 0; // Off

void checkSearchLightBtnClicked() {
    auto searchLightBtnClick = digitalRead(PIN_SEARCH_LIGHT_BUTTON);
    bool changed = false;
    if ((searchLightBtnClick == HIGH) && (lastSearchLightButtonState == LOW)) {
        changed = true;
    }
    if(changed) {
        searchLightBright = (searchLightBright + 1) % 3;
        switch(searchLightBright) {
            case 0:
                ledcWrite(CH_SEARCH_LIGHT, 0);
                ESP_LOGD(SEARCH_LIGHT_TAG, "Light off");
                break;
            case 1:
                ledcWrite(CH_SEARCH_LIGHT, 127);
                ESP_LOGD(SEARCH_LIGHT_TAG, "Light on(1)");
                break;
            case 2:
                ledcWrite(CH_SEARCH_LIGHT, 255);
                ESP_LOGD(SEARCH_LIGHT_TAG, "Light on(2)");
                break;
            default:
                ESP_LOGE(SEARCH_LIGHT_TAG, "Unhandled");
        }
    }

    lastSearchLightButtonState = searchLightBtnClick;
}
