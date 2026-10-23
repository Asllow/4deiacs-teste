#pragma once

#include <string>

namespace deiacs {
    class IFunctionBlock {
    public:
        virtual ~IFunctionBlock() = default;

        virtual bool initialize() = 0;

        virtual std::string getId() const = 0;

        virtual void* getDataOutput(const std::string& port_name) = 0;

        virtual bool connectDataInput(const std::string& port_name, void* data_pointer) = 0;

        virtual void triggerEventInput(const std::string& event_name) = 0;
    };
}