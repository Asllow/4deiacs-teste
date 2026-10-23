#pragma once
#include "esp_err.h"
#include "esp_event.h"
#include <string>

namespace deiacs{

ESP_EVENT_DECLARE_BASE(DEIACS_CORE_EVENTS);

class DeiacsEngine {

public:
    static esp_err_t start();

    static esp_err_t loadHardcodedMesh();

    static esp_err_t postEvent(int32_t event_id, void* event_data, size_t event_data_size);

    static esp_err_t subscribeEvent(int32_t event_id, esp_event_handler_t event_handler, void* event_handler_arg);

};

}