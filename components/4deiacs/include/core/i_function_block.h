#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace deiacs {
struct EventConnection {
    class IFunctionBlock* target_block;
    std::string target_port;
};

class IFunctionBlock {
public:
    IFunctionBlock(const std::string& id) : m_id(id) {}

    virtual ~IFunctionBlock() = default;

    virtual bool initialize() = 0;

    std::string getId() const { return m_id; }

    virtual void* getDataOutput(const std::string& port_name) = 0;

    virtual bool connectDataInput(const std::string& port_name, void* data_pointer) = 0;

    virtual void triggerEventInput(const std::string& event_name) = 0;

    void addEventOutputConnection(const std::string& output_event, IFunctionBlock* target, const std::string& target_event) {
        if (target != nullptr) {
            m_event_Outputs[output_event].push_back({target, target_event});
        }
    }
protected:
    void triggerEventOutput(const std::string& output_event) {
        auto it = m_event_Outputs.find(output_event);
        if (it != m_event_Outputs.end()) {
            for (const auto& conn : it->second) {
                conn.target_block->triggerEventInput(conn.target_port);
            }
        }
    }

    std::string m_id;

private:
    std::unordered_map<std::string, std::vector<EventConnection>> m_event_Outputs;
};
}