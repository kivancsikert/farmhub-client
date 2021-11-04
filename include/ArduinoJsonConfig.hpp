#pragma once

#include <ArduinoJson.h>
#include <functional>
#include <list>

using std::list;
using std::ref;
using std::reference_wrapper;

namespace farmhub { namespace client {

class BaseProperty {
public:
    BaseProperty(const char* name)
        : name(name) {
    }

    virtual void load(const JsonObject& json) = 0;
    virtual void store(JsonObject& json, bool maskSecrets) const = 0;

protected:
    const char* name;
};

template <typename T>
class Property : public BaseProperty {
public:
    Property(const char* name, const T& value, const bool secret = false)
        : BaseProperty(name)
        , secret(secret)
        , value(value) {
    }

    void set(const T& value) const {
        this->value = value;
    }

    const T& get() const {
        return value;
    }

    void load(const JsonObject& json) override {
        if (json.containsKey(name)) {
            value = json[name].as<T>();
        }
    }

    void store(JsonObject& json, bool maskSecrets) const override {
        if (maskSecrets && secret) {
            json[name] = "********";
        } else {
            json[name] = value;
        }
    }

private:
    const bool secret;
    T value;
};

class ConfigurationSerializer {
public:
    void add(BaseProperty& property) {
        auto reference = std::ref(property);
        properties.push_back(reference);
    }

    void load(const JsonObject& json) const {
        for (auto& property : properties) {
            property.get().load(json);
        }
    }

    void store(JsonObject& json) const {
        storeInternal(json, false);
    }

    void storeMaskingSecrets(JsonObject& json) const {
        storeInternal(json, true);
    }

private:
    void storeInternal(JsonObject& json, bool maskSecrets) const {
        for (auto& property : properties) {
            property.get().store(json, maskSecrets);
        }
    }

    list<reference_wrapper<BaseProperty>> properties;
};

}}    // namespace farmhub::client
