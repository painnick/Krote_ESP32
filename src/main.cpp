#include <Arduino.h>

#include "esp_log.h"

#include <ESP32Servo.h>

#include "Mp3Controller.h"

#define MAIN_TAG "Main"

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33
// for the ESP32-S2 the GPIO pins are 1-21,26,33-42

#define PIN_MP3_TX 23 // dfplayer(FIXED)
#define PIN_MP3_RX 32 // dfplayer(FIXED)
#define PIN_MP3_BUSY 35

#define PIN_SENSOR_LIGHT 19
#define CH_SENSOR_LIGHT 12

#define PIN_SEARCH_LIGHT 21
#define PIN_SEARCH_BUTTON 22

#define PIN_GATLING_LIGHT 15
#define PIN_GATLING_BUTTON 34

#define PIN_TURRET_SERVO 26

#define PIN_RADAR_SERVO 13

#define PIN_STEP_BUTTON 36


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
        dfmp3.playRandomTrackFromAll();
        dfmp3.loop();

        uint16_t cnt = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
#ifdef DEBUG
        ESP_LOGI(MAIN_TAG, "Track count : %d", cnt);
#endif
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

    dfmp3.playMp3FolderTrack(2);
    dfmp3.loop();
//    dfmp3.setRepeatPlayCurrentTrack(true);
//    dfmp3.loop();

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
