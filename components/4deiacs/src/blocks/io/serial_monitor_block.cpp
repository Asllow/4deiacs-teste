#include "serial_monitor_block.h"
#include "block_registry.h"
#include "esp_log.h"
#include <cstdio>

namespace deiacs {

static const char* TAG = "SERIAL_MONITOR";

SerialMonitorBlock::SerialMonitorBlock(const std::string& block_id, int num_inputs)
    : IFunctionBlock(block_id), m_num_inputs(num_inputs) {
    m_inputs.resize(num_inputs, nullptr);
}

bool SerialMonitorBlock::initialize() {
    ESP_LOGI(TAG, "[%s] Monitor Serial inicializado com %d portas de entrada.", m_id.c_str(), m_num_inputs);
    return true;
}

void* SerialMonitorBlock::getDataOutput(const std::string& port_name) {
    return nullptr;
}

bool SerialMonitorBlock::connectDataInput(const std::string& port_name, void* data_ptr) {
    if (port_name == "DATA_IN" && m_num_inputs >= 1) {
        m_inputs[0] = static_cast<float*>(data_ptr);
        return true;
    }

    if (port_name.rfind("IN_", 0) == 0) {
        int index = std::stoi(port_name.substr(3));
        if (index >= 1 && index <= m_num_inputs) {
            m_inputs[index - 1] = static_cast<float*>(data_ptr);
            return true;
        } else if (index == 0 && m_num_inputs >= 1) {
            m_inputs[0] = static_cast<float*>(data_ptr);
            return true;
        }
    }
    return false;
}

void SerialMonitorBlock::triggerEventInput(const std::string& event_name) {
    if (event_name == "REQ") {
        std::string output = ">>> [" + m_id + "] ";
        char buffer[64];

        for (int i = 0; i < m_num_inputs; ++i) {
            float val = (m_inputs[i] != nullptr) ? *(m_inputs[i]) : 0.0f;
            snprintf(buffer, sizeof(buffer), "IN_%d: %.2f | ", i + 1, val);
            output += buffer;
        }

        ESP_LOGI(TAG, "%s", output.c_str());
        triggerEventOutput("CNF");
    }
}

IFunctionBlock* SerialMonitorBlock::create(const std::string& block_id, cJSON* config) {
    cJSON* num_in_item = cJSON_GetObjectItem(config, "num_in");
    if (!num_in_item) {
        num_in_item = cJSON_GetObjectItem(config, "num_inputs");
    }
    
    int num_inputs = (num_in_item && cJSON_IsNumber(num_in_item)) ? num_in_item->valueint : 1;
    return new SerialMonitorBlock(block_id, num_inputs);
}

static bool registered = []() {
    BlockRegistry::registerBlock("SerialMonitor", SerialMonitorBlock::create);
    return true;
}();

}