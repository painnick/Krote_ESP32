#pragma once

#include <Arduino.h>
#include <esp_log.h>

#include "common.h"
#include "Mp3Controller.h"
#include "SensorController.h"
#include "RadarController.h"

#define GATLING_TAG "Gatling"

bool isGatlingOn = false;

void taskGatling(__attribute__((unused)) void *params) {
    ESP_LOGD(GATLING_TAG, "Fire!");

    suspendRadar();
    suspendSensor();

    isGatlingOn = true;

    dfmp3.setVolume(25);
    dfmp3.loop();

    dfmp3.playAdvertisement(1);
    dfmp3.loop();

    delay(500);

    ledcWrite(CH_GATLING_MOTOR, 255);

    for (int i = 0; i < 20; i++) {
        digitalWrite(PIN_GATLING_LIGHT, HIGH);
        delay(30);
        digitalWrite(PIN_GATLING_LIGHT, LOW);
        delay(30);
    }

    ledcWrite(CH_GATLING_MOTOR, 0);

    setDefaultVolume();

    dfmp3.stopAdvertisement();
    dfmp3.loop();

    isGatlingOn = false;

    resumeRadar();
    resumeSensor();

    vTaskDelete(nullptr);
}

void initGatling() {
    pinMode(PIN_GATLING_BUTTON, INPUT);
    pinMode(PIN_GATLING_LIGHT, OUTPUT);

    ledcSetup(CH_GATLING_MOTOR, 1000, 8);
    ledcAttachPin(PIN_GATLING_MOTOR, CH_GATLING_MOTOR);

}

int lastGatlingButtonState = HIGH;

void checkGatlingBtnClicked() {
    auto gatlingBtnClick = digitalRead(PIN_GATLING_BUTTON);
    if (gatlingBtnClick == HIGH) {
        if (lastGatlingButtonState == LOW) {
            ESP_LOGI(GATLING_TAG, "Click!");
            if (!isGatlingOn) {
                xTaskCreate(
                        taskGatling,
                        "Gatling",
                        10000,
                        nullptr,
                        1,
                        nullptr);
            }
        }
    }
    lastGatlingButtonState = gatlingBtnClick;
}