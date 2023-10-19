#include <Arduino.h>
#include <esp_log.h>
#include <ESP32Servo.h>

#include "common.h"
#include "Mp3Controller.h"
#include "SensorController.h"

#define MAIN_TAG "Main"

Servo turretServo;


TaskHandle_t radarTaskHandle;
TaskHandle_t turretTaskHandle;

[[noreturn]] void taskRadar(__attribute__((unused)) void *params) {
    while (true) {
        ESP_LOGI(MAIN_TAG, "Radar");

        for (int i = 0; i < 20; i++) {
            delay(1000);
        }
        ledcWrite(CH_RADAR_MOTOR, 47);
        delay(1000 * 1);
        ledcWrite(CH_RADAR_MOTOR, 0);
    }
}

[[noreturn]] void taskTurret(__attribute__((unused)) void *params) {
    while (true) {
        ESP_LOGI(MAIN_TAG, "Turret move");

        turretServo.write(random() % 30);
        delay(1000 * 10);
    }
}

bool isGatlingOn = false;

void taskGatling(__attribute__((unused)) void *params) {
    ESP_LOGI(MAIN_TAG, "Fire!");

    vTaskSuspend(radarTaskHandle);
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

    vTaskResume(radarTaskHandle);
    resumeSensor();

    vTaskDelete(nullptr);
}

void setup() {
#ifdef DEBUG
    ESP_LOGI(MAIN_TAG, "Setup...");
#endif
    // Gatling
    pinMode(PIN_GATLING_BUTTON, INPUT);
    pinMode(PIN_GATLING_LIGHT, OUTPUT);

    ledcSetup(CH_GATLING_MOTOR, 1000, 8);
    ledcAttachPin(PIN_GATLING_MOTOR, CH_GATLING_MOTOR);

    // Sensor Light
    initSensor();

    // Search Light
    pinMode(PIN_SEARCH_BUTTON, INPUT);
    pinMode(PIN_SEARCH_LIGHT, OUTPUT);

    // Turret
    turretServo.attach(PIN_TURRET_SERVO, 500, 2500);
    turretServo.write(0);

    xTaskCreate(
            taskTurret,
            "Turret",
            10000,
            nullptr,
            1,
            &turretTaskHandle);

    // Radar
    ledcSetup(CH_RADAR_MOTOR, 1000, 8);
    ledcAttachPin(PIN_RADAR_MOTOR, CH_RADAR_MOTOR);

    xTaskCreate(
            taskRadar,
            "Radar",
            10000,
            nullptr,
            3,
            &radarTaskHandle);

    // Sound
    pinMode(PIN_MP3_BUSY, INPUT);

    setupSound();
    delay(1000);

    playBackground();
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
