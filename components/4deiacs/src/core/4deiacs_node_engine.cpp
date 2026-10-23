#include "core/4deiacs_node_engine.h"
#include "core/management/block_registry.h"
#include "esp_log.h"
#include "cJSON.h"

namespace deiacs {

ESP_EVENT_DEFINE_BASE(DEIACS_CORE_EVENTS);
static const char* TAG = "DEIACS_ENGINE";

esp_err_t DeiacsEngine::start() {
    esp_err_t ret = esp_event_loop_create_default();
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Falha ao iniciar o loop de eventos.");
        return ret;
    }
    ESP_LOGI(TAG, "Motor IEC 61499 acoplado ao esp_event nativo.");
    return ESP_OK;
}

esp_err_t DeiacsEngine::postEvent(int32_t event_id, void* event_data, size_t event_data_size) {
    return esp_event_post(DEIACS_CORE_EVENTS, event_id, event_data, event_data_size, portMAX_DELAY);
}

esp_err_t DeiacsEngine::subscribeEvent(int32_t event_id, esp_event_handler_t event_handler, void* event_handler_arg) {
    return esp_event_handler_register(DEIACS_CORE_EVENTS, event_id, event_handler, event_handler_arg);
}

esp_err_t DeiacsEngine::loadHardcodedMesh() {
    ESP_LOGI(TAG, "Carregando malha hardcoded (Fase 1)...");
    const char* json_str = R"({
        "blocks": [
            { "id": "CLOCK_1", "type": "ECycle", "config": { "period": 1000 } },
            { "id": "MONITOR_1", "type": "SerialMonitor", "config": {} }
        ]
    })";

    cJSON* root = cJSON_Parse(json_str);
    if (!root) return ESP_FAIL;

    cJSON* blocks_array = cJSON_GetObjectItem(root, "blocks");
    cJSON* block_item = nullptr;

    cJSON_ArrayForEach(block_item, blocks_array) {
        cJSON* id_item = cJSON_GetObjectItem(block_item, "id");
        cJSON* type_item = cJSON_GetObjectItem(block_item, "type");
        if (!cJSON_IsString(id_item) || !cJSON_IsString(type_item)) {
            continue;
        }
        std::string b_id = id_item->valuestring;
        std::string b_type = type_item->valuestring;
        cJSON* b_config = cJSON_GetObjectItem(block_item, "config");
        
        IFunctionBlock* block = BlockRegistry::createBlock(b_type, b_id, b_config);
        if (block) {
            block->initialize();
        }
    }
    
    cJSON_Delete(root);
    return ESP_OK;
}

} // namespace deiacs
