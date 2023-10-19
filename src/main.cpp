#include <Arduino.h>
#include <esp_log.h>

#include "common.h"
#include "Mp3Controller.h"
#include "SensorController.h"
#include "RadarController.h"
#include "TurretController.h"
#include "GatlingController.h"

#define MAIN_TAG "Main"


void setup() {
    ESP_LOGI(MAIN_TAG, "Setup...");

    // Gatling
    initGatling();

    // Sensor Light
    initSensor();

    // Search Light
    pinMode(PIN_SEARCH_BUTTON, INPUT);
    pinMode(PIN_SEARCH_LIGHT, OUTPUT);

    // Turret
    initTurret();

    // Radar
    initRadar();

    // Sound
    pinMode(PIN_MP3_BUSY, INPUT);

    setupSound();
    delay(1000);

    playBackground();
}

void loop() {
    checkGatlingBtnClicked();

    dfmp3.loop();
}
