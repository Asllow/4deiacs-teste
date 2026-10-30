#pragma once

#include <string>
#include <vector>
#include "i_function_block.h"
#include "esp_err.h"

namespace deiacs{

class JsonParser {
public:
    static esp_err_t parseManifest(const char* json_payload);
};
}