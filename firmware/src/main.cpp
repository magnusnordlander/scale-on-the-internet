#include <Arduino.h>
#include <U8g2lib.h>
#include "../lib/panorama/src/NavigationController.h"
#include "../lib/panorama/src/ViewControllers/Menus/MenuViewController.h"
#include "ViewControllers/Settings/ScaleDebugViewController.h"
#include "ViewControllers/ScaleViewController.h"
#include "Settings.h"
#include "ScaleWrapper.h"
#include <types.h>

// DT and CLK must be interrupt pins
#define ENCODER_SW_PIN 4
#define ENCODER_DT_PIN 3
#define ENCODER_CLK_PIN 2

#define MANUAL_GRIND_PIN 14

// DOUT must be interrupt pin
#define ADS1232_PWDN_PIN 17
#define ADS1232_DOUT_PIN 15
#define ADS1232_SCLK_PIN 16
#define ADS1232_TEMP_PIN 6
#define ADS1232_A0_PIN 7
#define ADS1232_GAIN0_PIN 8
#define ADS1232_GAIN1_PIN 9
#define ADS1232_SPEED_PIN 20

// With the exception of CS, these pins are hardware supported
#define UEXT_SPI_CS_PIN 10
#define UEXT_SPI_SCK_PIN 13
#define UEXT_SPI_MISO_PIN 12
#define UEXT_SPI_MOSI_PIN 11
#define UEXT_I2C_SCL_PIN 19
#define UEXT_I2C_SDA_PIN 18

// Use I2C SCL as SSD1309 DC and I2C SDA as SSD1309 RES
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2, U8X8_PIN_NONE);

microtime_t Button_Press_Started_At = 0;

int Encoder_SW_State = 0;
ButtonEvent currentButtonEvent = BUTTON_INACTIVE;

ScaleWrapper* scale;
NavigationController* nav = nullptr;

void setup(void) {
    u8g2.begin();

    Serial.begin(115200);

    auto settings = new Settings();

    pinMode(ENCODER_SW_PIN, INPUT);

    pinMode(ADS1232_TEMP_PIN, OUTPUT);
    pinMode(ADS1232_A0_PIN, OUTPUT);
    pinMode(ADS1232_GAIN0_PIN, OUTPUT);
    pinMode(ADS1232_GAIN1_PIN, OUTPUT);
    pinMode(ADS1232_SPEED_PIN, OUTPUT);

    digitalWrite(ADS1232_TEMP_PIN, LOW);
    digitalWrite(ADS1232_A0_PIN, LOW);
    digitalWrite(ADS1232_GAIN0_PIN, HIGH);
    digitalWrite(ADS1232_GAIN1_PIN, HIGH);
    digitalWrite(ADS1232_SPEED_PIN, HIGH);

    scale = ScaleWrapper::GetInstance(ADS1232_DOUT_PIN, ADS1232_SCLK_PIN, ADS1232_PWDN_PIN, settings);

    nav = new NavigationController(new ScaleViewController(scale));
}

void updateExternalState() {
    Encoder_SW_State = digitalRead(ENCODER_SW_PIN);

    if (Button_Press_Started_At == 0 && Encoder_SW_State == LOW) {
        Button_Press_Started_At = micros();
        currentButtonEvent = BUTTON_PRESS;
    } else if (Button_Press_Started_At > 0 && Encoder_SW_State == LOW) {
        currentButtonEvent = BUTTON_HOLD;
    } else if (Button_Press_Started_At > 0 && Encoder_SW_State == HIGH) {
        microtime_t holdLength = micros() - Button_Press_Started_At;
        if (holdLength > 300000) {
            currentButtonEvent = BUTTON_PRESS_AND_HOLD_LET_UP;
        } else {
            currentButtonEvent = BUTTON_LET_UP;
        }
        Button_Press_Started_At = 0;
    } else { // (Button_Press_Started_At == 0 && Encoder_SW_State == HIGH) {
        currentButtonEvent = BUTTON_INACTIVE;
    }

    scale->refresh();
}

void loop(void) {
    updateExternalState();

    nav->top()->handleButtonEvent(currentButtonEvent);

    nav->top()->tick(u8g2);
}