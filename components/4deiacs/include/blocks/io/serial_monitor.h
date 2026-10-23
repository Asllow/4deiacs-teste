#pragma once

#include "core/i_function_block.h"
#include "cJSON.h"
#include <map>

namespace deiacs {

class SerialMonitorBlock : public IFunctionBlock {
public:
    SerialMonitorBlock(const std::string& id, cJSON* config);
    ~SerialMonitorBlock() override = default;

    bool initialize() override;
    std::string getId() const override { return id_; }

    void* getDataOutput(const std::string& port_name) override { return nullptr; }
    bool connectDataInput(const std::string& port_name, void* data_pointer) override;
    void triggerEventInput(const std::string& event_name) override;

private:
    std::string id_;
    std::map<std::string, void*> input_pins_;
};

} // namespace deiacs
