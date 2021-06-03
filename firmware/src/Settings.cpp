//
// Created by Magnus Nordlander on 2021-02-06.
//

#include "Settings.h"

FlashStorage(settingsStorage, SettingsStorageStruct);

float Settings::getScaleCalibration() const {
    return this->scaleCalibration;
}

void Settings::setScaleCalibration(float scaleCalibration) {
    this->scaleCalibration = scaleCalibration;
}

Settings::Settings() {
    this->savedStorage = settingsStorage.read();

    if (!this->savedStorage.valid) {
        this->savedStorage = (SettingsStorageStruct){
                .valid =  false,
                .scaleCalibration = 430.0,
        };
    }

    this->scaleCalibration = this->savedStorage.scaleCalibration;
}

void Settings::commitToEEPROM() {
    if (
        this->scaleCalibration != this->savedStorage.scaleCalibration
            ) {
        this->savedStorage = (SettingsStorageStruct){
                .valid = true,
                .scaleCalibration = this->scaleCalibration,
        };

        settingsStorage.write(this->savedStorage);
    }
}
