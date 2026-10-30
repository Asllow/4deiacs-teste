#include "e_cycle_block.h"
#include "block_registry.h"
#include "esp_log.h"

namespace deiacs {

static const char* TAG = "E_CYCLE_BLOCK";

ECycleBlock::ECycleBlock(const std::string& block_id, uint64_t period_ms)
    : IFunctionBlock(block_id), 
      m_period_us(period_ms * 1000), 
      m_timer_handle(nullptr), 
      m_is_running(false),
      m_tick_count(0.0f) {}

ECycleBlock::~ECycleBlock() {
    stop();
    if (m_timer_handle != nullptr) {
        esp_timer_delete(m_timer_handle);
    }
}

bool ECycleBlock::initialize() {
    esp_timer_create_args_t timer_args = {};
    timer_args.callback = &ECycleBlock::timerCallback;
    timer_args.arg = this;
    timer_args.name = m_id.c_str();
    timer_args.dispatch_method = ESP_TIMER_TASK;

    esp_err_t err = esp_timer_create(&timer_args, &m_timer_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "[%s] Failed to create esp_timer.", m_id.c_str());
        return false;
    }

    ESP_LOGI(TAG, "[%s] Timer initialized (Period: %llu ms).", m_id.c_str(), m_period_us / 1000);
    return true;
}

bool ECycleBlock::start() {
    if (m_timer_handle == nullptr || m_is_running) {
        return false;
    }

    esp_err_t err = esp_timer_start_periodic(m_timer_handle, m_period_us);
    if (err == ESP_OK) {
        m_is_running = true;
        ESP_LOGI(TAG, "[%s] Timer started.", m_id.c_str());
        return true;
    }
    return false;
}

bool ECycleBlock::stop() {
    if (m_timer_handle == nullptr || !m_is_running) {
        return false;
    }

    esp_err_t err = esp_timer_stop(m_timer_handle);
    if (err == ESP_OK) {
        m_is_running = false;
        ESP_LOGI(TAG, "[%s] Timer stopped.", m_id.c_str());
        return true;
    }
    return false;
}

void ECycleBlock::timerCallback(void* arg) {
    ECycleBlock* instance = static_cast<ECycleBlock*>(arg);
    if (instance != nullptr) {
        instance->m_tick_count += 1.0f;
        instance->triggerEventOutput("EO");
        instance->triggerEventOutput("EV_OUT");
    }
}

void ECycleBlock::triggerEventInput(const std::string& event_name) {
    if (event_name == "START") {
        start();
    } else if (event_name == "STOP") {
        stop();
    }
}

void* ECycleBlock::getDataOutput(const std::string& port_name) {
    if (port_name == "DATA_OUT") {
        return &m_tick_count;
    }
    return nullptr;
}

bool ECycleBlock::connectDataInput(const std::string& port_name, void* data_ptr) {
    return false;
}

IFunctionBlock* ECycleBlock::create(const std::string& block_id, cJSON* config) {
    cJSON* period = cJSON_GetObjectItem(config, "period_ms");
    uint64_t period_ms = (period != nullptr) ? period->valueint : 1000;

    return new ECycleBlock(block_id, period_ms);
}

static bool is_registered = []() {
    BlockRegistry::registerBlock("ECycle", ECycleBlock::create);
    return true;
}();

}