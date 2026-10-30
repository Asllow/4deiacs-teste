#pragma once

#include <string>
#include <vector>
#include "i_function_block.h"
#include "cJSON.h"

namespace deiacs {

class SerialMonitorBlock : public IFunctionBlock {
public:
    SerialMonitorBlock(const std::string& block_id, int num_inputs);
    ~SerialMonitorBlock() override = default;

    bool initialize() override;

    void* getDataOutput(const std::string& port_name) override;
    bool connectDataInput(const std::string& port_name, void* data_ptr) override;
    void triggerEventInput(const std::string& event_name) override;
    static IFunctionBlock* create(const std::string& block_id, cJSON* config);
private:
    int m_num_inputs;
    std::vector<float*> m_inputs;
};

} // namespace deiacs
