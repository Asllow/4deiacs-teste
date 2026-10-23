#pragma once

#include "core/i_function_block.h"
#include "cJSON.h"
#include "esp_timer.h"

namespace deiacs{

class ECycleBlock : public IFunctionBlock {
    public:
        ECycleBlock(const std::string& id, cJSON* config);
        ~ECycleBlock() override;

        bool initialize() override;
        std::string getId() const override { return id_; }

        void* getDataOutput(const std::string& port_name) override;
        bool connectDataInput(const std::string& port_name, void* data_pointer) override;
        void triggerEventInput(const std::string& event_name) override;
    private:
        std::string id_;
        uint64_t period_ms_;
        esp_timer_handle_t timer_handle_;

        void startTimer();

        static void timerCallback(void* arg);

};
}