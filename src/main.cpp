#include <Arduino.h>
#include <esp_log.h>
#include <ESP32Servo.h>

#include "common.h"
#include "Mp3Controller.h"

#define MAIN_TAG "Main"

Servo turretServo;
Servo radarServo;

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

void setup() {
#ifdef DEBUG
    ESP_LOGI(MAIN_TAG, "Setup...");
#endif
    pinMode(PIN_GATLING_LIGHT, OUTPUT);

    ledcSetup(CH_SENSOR_LIGHT, 1000, 8);
    ledcAttachPin(PIN_SENSOR_LIGHT, CH_SENSOR_LIGHT);

    setupSound();
    delay(1000);

    playBackground();

    radarServo.attach(PIN_RADAR_SERVO);
    radarServo.write(30);

    turretServo.attach(PIN_TURRET_SERVO);
    turretServo.write(0);

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

void loop() {
//    uint16_t val = analogRead(PIN_GATLING_BUTTON);
//    if (val < 255) {
//#ifdef DEBUG
//        ESP_LOGI(MAIN_TAG, "Click!");
//#endif
//        dfmp3.playAdvertisement(1);
//        dfmp3.loop();
//
//        for (int i = 0; i < 5; i++) {
//            digitalWrite(PIN_GATLING_LIGHT, HIGH);
//            delay(100);
//            digitalWrite(PIN_GATLING_LIGHT, LOW);
//            delay(100);
//        }
//        dfmp3.stopAdvertisement();

//#ifdef DEBUG
//        static int count = 0;
//        auto isExplosion = (++count) % 3 == 1;
//#else
//        auto isExplosion = random(0, 5) == 1;
//#endif
//        if (isExplosion) {
//#ifdef DEBUG
//            ESP_LOGI(MAIN_TAG, "Explosion!!!");
//#endif
//            delay(1000);
//            dfmp3.playAdvertisement(2);
//            dfmp3.loop();
//            delay(1200);
//            dfmp3.stopAdvertisement();
//            dfmp3.loop();
//            delay(1500);
//        }
//    }

    dfmp3.loop();
}
