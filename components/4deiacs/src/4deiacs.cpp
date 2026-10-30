#include "4deiacs.h"
#include "4deiacs_node_engine.h"
#include "esp_log.h"

static const char* TAG = "4DEIACS_MAIN";

void init_4deiacs(void) {
    ESP_LOGI(TAG, "Inicializando o ecossistema 4deiacs (Etapa 1 - JSON Hardcoded)...");

    if (deiacs::DeiacsEngine::start() != ESP_OK) {
        ESP_LOGE(TAG, "Falha critica ao iniciar o motor de eventos. Abortando.");
        return;
    }

    ESP_LOGI(TAG, "4deiacs em operacao.");
}