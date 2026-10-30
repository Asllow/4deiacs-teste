#pragma once

#include "esp_err.h"
#include "deiacs_events.h"
#include <string>

namespace deiacs{

class DeiacsEngine {

public:
    static esp_err_t start();

    static esp_err_t loadHardcodedMesh();

    static esp_err_t stopAndClearMesh();

    static esp_err_t postEvent(EventIds event_id, void* event_data = nullptr, size_t event_data_size = 0);

    static esp_err_t subscribeEvent(EventIds event_id, esp_event_handler_t event_handler, void* event_handler_arg);
};

}