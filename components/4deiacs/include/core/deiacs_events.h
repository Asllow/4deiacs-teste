#pragma once

#include "esp_event.h"

namespace deiacs {

ESP_EVENT_DECLARE_BASE(DEIACS_CORE_EVENTS);

/**
 * @brief Identificadores de eventos globais do barramento interno do motor.
 */
enum EventIds {
    EV_INIT = 0,
    EV_START_ROUTINE,
    EV_SENSOR_READ_REQ,
    EV_SENSOR_READ_CNF,
    EV_CALC_PID_REQ,
    EV_CALC_PID_CNF,
    EV_PWM_UPDATE_REQ,
    EV_PWM_UPDATE_CNF,
    EV_NETWORK_CONNECTED,
    EV_NET_TX_REQ,
    EV_NET_RX_IND,
    EV_ERROR_DETECTED
};

}