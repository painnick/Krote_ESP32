#include <Arduino.h>
#include <esp_log.h>
#include <ESP32Servo.h>

#include "common.h"
#include "Mp3Controller.h"

#define MAIN_TAG "Main"

Servo turretServo;
Servo radarServo;
Servo gatlingServo;

[[noreturn]] void taskSensor(__attribute__((unused)) void *params) {
    while (true) {
        delay(10);
    }
}

[[noreturn]] void taskTurret(__attribute__((unused)) void *params) {
    while (true) {
#ifdef DEBUG
        ESP_LOGI(MAIN_TAG, "Turret move");
#endif
        turretServo.write(random() % 30);
        delay(1000 * 10);
    }
}

bool isGatlingOn = false;

void taskGatling(__attribute__((unused)) void *params) {
    isGatlingOn = true;
    dfmp3.playAdvertisement(1);
    dfmp3.loop();

    gatlingServo.write(30);

    for (int i = 0; i < 5; i++) {
        digitalWrite(PIN_GATLING_LIGHT, HIGH);
        delay(100);
        digitalWrite(PIN_GATLING_LIGHT, LOW);
        delay(100);
    }

    gatlingServo.write(0);

    dfmp3.stopAdvertisement();
    isGatlingOn = false;
    vTaskDelete(nullptr);
}

void setup() {
#ifdef DEBUG
    ESP_LOGI(MAIN_TAG, "Setup...");
#endif
    pinMode(PIN_GATLING_LIGHT, OUTPUT);

    ledcSetup(CH_SENSOR_LIGHT, 1000, 8);
    ledcAttachPin(PIN_SENSOR_LIGHT, CH_SENSOR_LIGHT);

    pinMode(PIN_GATLING_BUTTON, INPUT);

    setupSound();
    delay(1000);

    playBackground();

    radarServo.attach(PIN_RADAR_SERVO);

    turretServo.attach(PIN_TURRET_SERVO);
    turretServo.write(0);

    gatlingServo.attach(PIN_GATLING_SERVO);

    xTaskCreate(
            taskSensor,
            "Sensor",
            10000,
            nullptr,
            1,
            nullptr);

    xTaskCreate(
            taskTurret,
            "Turret",
            10000,
            nullptr,
            1,
            nullptr);
}

int lastGatlingButtonState = HIGH;

void loop() {
    auto gatlingBtnClick = digitalRead(PIN_GATLING_BUTTON);
    if (gatlingBtnClick == HIGH) {
        if (lastGatlingButtonState == LOW) {
            ESP_LOGI(MAIN_TAG, "Click!");
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

    dfmp3.loop();
}
