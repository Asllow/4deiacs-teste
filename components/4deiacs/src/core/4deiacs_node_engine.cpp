#include "4deiacs_node_engine.h"
#include "block_registry.h"
#include "connection_manager.h"
#include "json_parser.h"
#include "esp_log.h"
#include "esp_event.h"

namespace deiacs {

ESP_EVENT_DEFINE_BASE(DEIACS_CORE_EVENTS);
static const char* TAG = "DEIACS_ENGINE";

static const char* HARDCODED_TEST_JSON = R"({
    "blocks": [
        {
            "id": "CLOCK_1",
            "type": "ECycle",
            "config": {
                "period_ms": 1000
            }
        },
        {
            "id": "MONITOR_1",
            "type": "SerialMonitor",
            "config": {
                "num_in": 1
            }
        }
    ],
    "connections": [
        {
            "source": "CLOCK_1.DATA_OUT",
            "target": "MONITOR_1.IN_1"
        },
        {
            "source": "CLOCK_1.EO",
            "target": "MONITOR_1.REQ"
        }
    ]
})";

esp_err_t DeiacsEngine::start() {
    ESP_LOGI(TAG, "Inicializando o Barramento de Eventos (4deiacs Engine)...");
    
    esp_err_t err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Falha critica ao criar Event Loop: %s", esp_err_to_name(err));
        return err;
    }

    return loadHardcodedMesh();
}

esp_err_t DeiacsEngine::postEvent(EventIds event_id, void* event_data, size_t event_data_size) {
    return esp_event_post(DEIACS_CORE_EVENTS, event_id, event_data, event_data_size, portMAX_DELAY);
}

esp_err_t DeiacsEngine::subscribeEvent(EventIds event_id, esp_event_handler_t event_handler, void* event_handler_arg) {
    return esp_event_handler_register(DEIACS_CORE_EVENTS, event_id, event_handler, event_handler_arg);
}

esp_err_t DeiacsEngine::loadHardcodedMesh() {
    ESP_LOGI(TAG, "Carregando malha JSON Hardcoded (Etapa 1)...");
    return JsonParser::parseManifest(HARDCODED_TEST_JSON);
}

esp_err_t DeiacsEngine::stopAndClearMesh() {
    ESP_LOGI(TAG, "Iniciando destruicao segura da malha...");
    ConnectionManager::clearAll();
    BlockRegistry::clearAll();
    return ESP_OK;
}

} // namespace deiacs
