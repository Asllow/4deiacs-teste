#include "serial_monitor.h"
#include "block_registry.h"
#include "esp_log.h"

namespace deiacs{

static const char* TAG = "SERIAL_MONITOR";

SerialMonitorBlock::SerialMonitorBlock(const std::string& id, cJSON* config) : id_(id) {
    // Inicializamos os pinos que queremos monitorizar com nullptr
    input_pins_["IN_0"] = nullptr;
    input_pins_["IN_1"] = nullptr;
}

bool SerialMonitorBlock::initialize() {
    ESP_LOGI(TAG, "[%s] Monitor Serial inicializado.", id_.c_str());
    return true;
}

bool SerialMonitorBlock::connectDataInput(const std::string& port_name, void* data_pointer) {
    if (input_pins_.find(port_name) != input_pins_.end()) {
        input_pins_[port_name] = data_pointer;
        return true;
    }
    ESP_LOGW(TAG, "[%s] Tentativa de conexao em porta inexistente: %s", id_.c_str(), port_name.c_str());
    return false;
}

void SerialMonitorBlock::triggerEventInput(const std::string& event_name) {
    if (event_name == "REQ") {
        float val_0 = 0.0f;
        float val_1 = 0.0f;

        // Se o fio estiver conectado (diferente de nullptr), fazemos o cast veloz para extrair o valor real
        if (input_pins_["IN_0"]) val_0 = *(static_cast<float*>(input_pins_["IN_0"]));
        if (input_pins_["IN_1"]) val_1 = *(static_cast<float*>(input_pins_["IN_1"]));

        ESP_LOGI(TAG, "[%s] Leitura: IN_0 = %f | IN_1 = %f", id_.c_str(), val_0, val_1);
    }
}

static bool registered = []() {
    BlockRegistry::registerBlock("SerialMonitor", [](const std::string& id, cJSON* config) -> IFunctionBlock* {
        return new SerialMonitorBlock(id, config);
    });
    return true;
}();

}