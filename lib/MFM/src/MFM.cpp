
#include "MFM.h"

MFMState MFM::state;

/*
 * Define static functions
 */
void MFM::Setup(CommunicationSAL & communication, bool automaticTrigger = true) {
    // Variables
    MFM::communication = &communication;
    MFM::automaticTrigger = automaticTrigger;

    // Space reservation
    Settings::Reset();

    // Register required EEPROM space
    MFM::LoadState(Settings::ReserveSpace(sizeof(MFMState)));
    setupSettings();
    
    // Register required middleware
    setupMiddleware(MFM::middleware);
    middleware.add(&(MFM::SendData));
}

void MFM::LoadState(int address) {
    EEPROM.readBlock(address, MFM::state);
}

bool MFM::SendData(SensorResultContext<SENSOR_MAX_ENTRIES>& context) {
    communication->activate();
    // INFO: For other context objects, make sure you convert them with the correct converter
    // SensorResultContextConverter converter;
    uint8_t dataBytes[] = {0};
    communication->send(dataBytes, 1);
}

bool MFM::LowPowerSleep() {
    // Enter LowPower mode (Multiple of 8)
    for (int r = state.triggerInterval; r > 0; r -= 8) {
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
}

void MFM::TriggerChain(SensorResultContext<SENSOR_MAX_ENTRIES> &context) {
    middleware.execute(context);
    LowPowerSleep();
}

int MFM::GetTriggerInterval() {
    return state.triggerInterval;
}