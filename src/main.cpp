#include <Arduino.h>
#include <esp_log.h>

#include "common.h"
#include "controllers/Mp3Controller.h"
#include "controllers/SensorController.h"
#include "controllers/RadarController.h"
#include "controllers/TurretController.h"
#include "controllers/GatlingController.h"
#include "controllers/StepController.h"
#include "controllers/SearchLightController.h"

#define MAIN_TAG "Main"

void setup() {
    ESP_LOGI(MAIN_TAG, "Setup...");

    setupGatling();
    setupSensor();
    setupRadar();
    setupTurret();
    setupStep();
    setupSearchLight();

    setupSound();

    delay(1000);
    playBackground();
}

void loop() {
    unsigned long now = millis();

    checkGatlingBtnClicked();
    checkStepBtnClicked();
    checkSearchLightBtnClicked();

    loopTurret(now);

    dfmp3.loop();
}
