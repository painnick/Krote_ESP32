#include <Arduino.h>
#include <esp_log.h>

#include "common.h"
#include "controllers/Mp3Controller.h"
#include "controllers/SensorController.h"
#include "controllers/RadarController.h"
#include "controllers/TurretController.h"
#include "controllers/GatlingController.h"

#define MAIN_TAG "Main"

void setup() {
    ESP_LOGI(MAIN_TAG, "Setup...");

    setupGatling();
    setupSensor();
    setupRadar();
    setupTurret();

    pinMode(PIN_SEARCH_BUTTON, INPUT);
    pinMode(PIN_SEARCH_LIGHT, OUTPUT);

    setupSound();

    delay(1000);
    playBackground();
}

void loop() {
    checkGatlingBtnClicked();

    loopTurret();

    dfmp3.loop();
}
