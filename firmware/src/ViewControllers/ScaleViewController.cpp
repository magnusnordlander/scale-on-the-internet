//
// Created by Magnus Nordlander on 2021-02-13.
//

#include <Utils/TextUtils.h>
#include "ScaleViewController.h"

ScaleViewController::ScaleViewController(ScaleWrapper *scale) : scale(scale) {
    this->progressBar = new ProgressBarView;
}

void ScaleViewController::render(U8G2 display) {
    if (this->taring) {
        display.setFont(u8g2_font_helvB12_te);
        display.drawStr(32,20, "Taring...");

        this->progressBar->drawRelative(display, 14, 28);
    } else {
        // Avoid -0.0
        if (displayValue < 0. && displayValue > -0.05) {
            drawLargeFloatWithUnits(display, 0., "g", 28);
        } else {
            drawLargeFloatWithUnits(display, displayValue, "g", 28);
        }

        if (this->scale->isValueStableHighAccuracy()) {
            display.drawDisc(5, 5, 3);
            display.drawDisc(9, 5, 3);
        } else if (this->scale->isValueStableLowAccuracy()) {
            display.drawDisc(5, 5, 3);
        }
    }
}

void ScaleViewController::handleButtonEvent(ButtonEvent event) {
    if (event == BUTTON_PRESS_AND_HOLD_LET_UP) {
        this->taring = true;
    } else if (event == BUTTON_LET_UP) {
        this->navigationController->pop();
    }
}

void ScaleViewController::tick(U8G2 display) {
    if (this->taring && this->scale->isValueStableHighAccuracy()) {
        this->scale->tare(2000000);
        this->taring = false;
    }

    float latest;

    if (this->scale->isValueStableHighAccuracy()) {
        latest = this->scale->getLatestWeightLongAverage();
    } else {
        latest = this->scale->getLatestWeightShortAverage();
    }

    if (tickCounter++ % 16 == 0 || fabs(latest - displayValue) > 0.2) {
        displayValue = latest;
    }

    BaseViewController::tick(display);
}

ScaleViewController::~ScaleViewController() {
    delete this->progressBar;
}
