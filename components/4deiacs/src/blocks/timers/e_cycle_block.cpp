#include "blocks/timers/e_cycle_block.h"
#include "core/management/block_registry.h"
#include "esp_log.h"
#include "esp_event.h"

namespace deiacs{

static const char* TAG = "E_CYCLE_BLOCK";

ECycleBlock::ECycleBlock(const std::string& id, cJSON* config) : id_(id), timer_handle_(nullptr) {
    period_ms_ = 1; 
    if (config) {
        cJSON* period_item = cJSON_GetObjectItem(config, "period");
        if (cJSON_IsNumber(period_item)) {
            period_ms_ = period_item->valueint;
        }
    }
}

ECycleBlock::~ECycleBlock() {
    if (timer_handle_) {
        esp_timer_stop(timer_handle_);
        esp_timer_delete(timer_handle_);
        ESP_LOGD(TAG, "[%s] Temporizador destruido (Teardown).", id_.c_str());
    }
}

bool ECycleBlock::initialize() {
    const esp_timer_create_args_t timer_args = {
        .callback = &ECycleBlock::timerCallback,
        .arg = this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = id_.c_str(),
        .skip_unhandled_events = true
    };

    if (esp_timer_create(&timer_args, &timer_handle_) != ESP_OK) {
        ESP_LOGE(TAG, "[%s] Falha ao criar o temporizador de hardware.", id_.c_str());
        return false;
    }

    ESP_LOGI(TAG, "[%s] Timer inicializado (Periodo: %llu ms).", id_.c_str(), period_ms_);
    
    startTimer(); 
    return true;
}

void ECycleBlock::startTimer() {
    if (timer_handle_) {
        esp_timer_start_periodic(timer_handle_, period_ms_ * 1000ULL);
    }
}

void ECycleBlock::timerCallback(void* arg) {
    ECycleBlock* block = static_cast<ECycleBlock*>(arg);
    ESP_LOGD(TAG, "[%s] Pulso EVENT_OUT disparado!", block->id_.c_str());
}

void* ECycleBlock::getDataOutput(const std::string& port_name) { return nullptr; }
bool ECycleBlock::connectDataInput(const std::string& port_name, void* data_pointer) { return false; }
void ECycleBlock::triggerEventInput(const std::string& event_name) {}

static bool registered = []() {
    BlockRegistry::registerBlock("ECycle", [](const std::string& id, cJSON* config) -> IFunctionBlock* {
        return new ECycleBlock(id, config);
    });
    return true;
}();

}