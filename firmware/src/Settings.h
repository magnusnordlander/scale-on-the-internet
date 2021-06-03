//
// Created by Magnus Nordlander on 2021-02-06.
//

#ifndef GRINDER_SETTINGS_H
#define GRINDER_SETTINGS_H

#include <FlashAsEEPROM.h>

typedef struct {
    bool valid;
    float scaleCalibration;
} SettingsStorageStruct;

class Settings {
public:
    explicit Settings();

    float getScaleCalibration() const;
    void setScaleCalibration(float scaleCalibration);

    void commitToEEPROM();
private:
    float scaleCalibration; // unitless

    SettingsStorageStruct savedStorage;
};


#endif //GRINDER_SETTINGS_H
