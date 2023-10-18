#pragma once

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33
// for the ESP32-S2 the GPIO pins are 1-21,26,33-42

#define PIN_MP3_TX 33 // dfplayer(FIXED)
#define PIN_MP3_RX 32 // dfplayer(FIXED)
#define PIN_MP3_BUSY 35

#define PIN_GATLING_BUTTON 34
#define PIN_GATLING_MOTOR 27
#define PIN_GATLING_LIGHT 22

#define PIN_SENSOR_LIGHT 23

#define PIN_SEARCH_BUTTON 39
#define PIN_SEARCH_LIGHT 21

#define PIN_TURRET_SERVO 25

#define PIN_RADAR_MOTOR 14



#define CH_GATLING_MOTOR 3
#define CH_RADAR_MOTOR 1
#define CH_SENSOR_LIGHT 2
