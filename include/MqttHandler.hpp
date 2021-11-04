#pragma once

#include <ArduinoJson.h>
#include <ArduinoJsonConfig.hpp>
#include <CircularBuffer.h>
#include <Client.h>
#include <ESPmDNS.h>
#include <MQTT.h>
#include <WiFiClient.h>
#include <chrono>
#include <functional>

#include <Loopable.hpp>

#define MQTT_BUFFER_SIZE 2048
#define MQTT_QUEUED_MESSAGES_MAX 16

using namespace std::chrono;

namespace farmhub { namespace client {

class MqttHandler
    : public TimedLoopable<void> {
public:
    MqttHandler();

    void begin(
        const JsonObject& mqttConfig,
        std::function<void(const JsonObject&)> onConfigChange,
        std::function<void(const JsonObject&)> onCommand);

    bool publish(const String& topic, const JsonDocument& json, bool retained = false, int qos = 0);
    bool subscribe(const String& topic, int qos);

protected:
    void timedLoop() override;
    milliseconds nextLoopAfter() const override {
        return milliseconds { 500 };
    }
    void defaultValue() override {
    }

private:
    bool tryConnect();

    WiFiClient client;
    MQTTClient mqttClient;

    Property<String> host;
    Property<int> port;
    Property<String> clientId;
    Property<String> prefix;
    ConfigurationSerializer serializer;

    bool connecting = false;

    struct MqttMessage {
        MqttMessage()
            : topic("")
            , payload("")
            , retain(false)
            , qos(0) {
        }

        MqttMessage(const String& topic, const JsonDocument& payload, boolean retain, int qos)
            : topic(topic)
            , retain(retain)
            , qos(qos) {
            serializeJson(payload, this->payload);
        }

        String topic;
        String payload;
        boolean retain;
        int qos;
    };

    CircularBuffer<MqttMessage, MQTT_QUEUED_MESSAGES_MAX> publishQueue;
};

}}    // namespace farmhub::client
