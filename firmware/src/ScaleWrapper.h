//
// Created by Magnus Nordlander on 2021-02-08.
//

#ifndef GRINDER_SCALEWRAPPER_H
#define GRINDER_SCALEWRAPPER_H

#include <deque>
#include <HX711_ADC.h>
#include "Settings.h"
#include <types.h>

#define HIGH_SPEED

#ifdef HIGH_SPEED
#define SPEED 80
#define SHORT_AVG_MS 100
#define LONG_AVG_MS 2000
#define STABILITY_LOW_RES_MS 500
#define STABILITY_LOW_RES_MIN_NUM 5
#define STABILITY_HIGH_RES_MS 2000
#define STABILITY_HIGH_RES_MIN_NUM 5
#define TARE_TIME_MS 1000
#define ROC_MS 1000
#define ROC_NUM 10
#else
#define SPEED 10
#define SHORT_AVG_MS 200
#define LONG_AVG_MS 2000
#define STABILITY_LOW_RES_MS 500
#define STABILITY_LOW_RES_MIN_NUM 3
#define STABILITY_HIGH_RES_MS 2000
#define STABILITY_HIGH_RES_MIN_NUM 3
#define TARE_TIME_MS 1000
#define ROC_MS 1000
#define ROC_NUM 3
#endif

#define STABILITY_LOW_RES_SIGMA 200
#define STABILITY_HIGH_RES_SIGMA 100

#define LATEST_VALUE_SIZE 50
#define RATE_OF_CHANGE_HISTORY 10


class ScaleDebugViewController;

typedef struct {
    int32_t measuringPoint;
    microtime_t microtime;
} MeasuringPoint;

class ScaleWrapper {
protected:
    ScaleWrapper(pin_size_t doutPin, pin_size_t clkPin, pin_size_t pdwnPin, Settings* settings);

    static ScaleWrapper* singleton_;

public:
    static ScaleWrapper *GetInstance(pin_size_t doutPin, pin_size_t clkPin, pin_size_t pdwnPin, Settings* settings);

    void refresh();
    void tare(microtime_t micros = 1000000);

    float measureCalibrationValue(float knownMass, microtime_t relMicros = 100000);

    bool isValueStableHighAccuracy();
    bool isValueStableLowAccuracy();
    float getLatestWeight();
    float getLatestWeightShortAverage();
    float getLatestWeightLongAverage();
    float getRateOfChange();

    static void dataReadyISR();
private:
    Settings* settings;

    pin_size_t doutPin, clkPin, pdwnPin;

    void init() const;
    int32_t readRaw() const;
    float convert(int32_t data) const;

    int32_t averageLast(microtime_t relMicros);
    MeasuringPoint firstValueSince(microtime_t relMicros);
    MeasuringPoint averagePointSince(microtime_t relMicros);
    MeasuringPoint averagePointSince(microtime_t relMicros, unsigned short num);
    MeasuringPoint latestAverage(unsigned short num);
    float _getRateOfChange();
    float _getLatestValue();

    float scaleStandardDeviation(microtime_t relMicros, unsigned short minValues);
    bool isValueStable(microtime_t relMicros, unsigned short minValues, uint32_t sigma);
    microtime_t bufSize();
    MeasuringPoint min();
    MeasuringPoint max();

    int32_t tareValue = 0;
    float calibrationValue = 0;

    MeasuringPoint safeData[LATEST_VALUE_SIZE];
    uint16_t safeHead = 0;
    bool safeFilledOnce = false;

    microtime_t latestRefreshed = 0;

    std::deque<MeasuringPoint>* latestValues;

    MeasuringPoint latestValue;
    MeasuringPoint latestValueShortAverage;
    MeasuringPoint latestValueLongAverage;

    float rateOfChange;
    float averageRateOfChange;
    std::deque<float>* previousRatesOfChange;

    friend class ScaleDebugViewController;
};


#endif //GRINDER_SCALEWRAPPER_H
