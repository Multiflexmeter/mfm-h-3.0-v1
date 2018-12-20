#ifndef _MFM_LORAWAN_H_
#define _MFM_LORAWAN_H_

#include <lmic.h>
#include <hal/hal.h>

#include <MFM.h>

// TODO: Configuration for other frequencies

class LoRaWan {
private:
    static osjob_t triggerJob;
public:
    static void Setup();
    static void Loop();
    static void Middleware(SensorResultContext<SENSOR_MAX_ENTRIES> &context);
    static void TriggerJob(osjob_t *job);
};

#endif