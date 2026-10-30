#pragma once

#include <string>
#include "i_function_block.h"
#include "esp_timer.h"
#include "cJSON.h"

namespace deiacs{

class ECycleBlock : public IFunctionBlock {
    public:
        ECycleBlock(const std::string& block_id, uint64_t period_ms);
        ~ECycleBlock() override;

        bool initialize() override;

        bool start();

        bool stop();

        void* getDataOutput(const std::string& port_name) override;
        bool connectDataInput(const std::string& port_name, void* data_ptr) override;
        void triggerEventInput(const std::string& event_name) override;

        static IFunctionBlock* create(const std::string& block_id, cJSON* config);
    private:
        uint64_t m_period_us;
        esp_timer_handle_t m_timer_handle;
        bool m_is_running;
        float m_tick_count;

        void startTimer();

        static void timerCallback(void* arg);

};
}