#include "core/4deiacs_node_engine.h"
#include "esp_log.h"

extern "C" void app_main(void) {
    ESP_LOGI("BOOT", "Iniciando Node Edge IEC 61499 (4deiacs)");
    
    // 1. Inicializa o barramento central de eventos
    if (deiacs::DeiacsEngine::start() == ESP_OK) {
        // 2. Acorda a malha de teste fixa
        deiacs::DeiacsEngine::loadHardcodedMesh();
    } else {
        ESP_LOGE("BOOT", "Falha critica (Kernel Panic). Sistema inoperante.");
    }
}