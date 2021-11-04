#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>

#include <Loopable.hpp>

namespace farmhub { namespace client {

class OtaHandler
    : public Loopable<void> {

public:
    OtaHandler() {
    }

    void begin(const char* hostname) {
        ArduinoOTA.setHostname(hostname);

        ArduinoOTA.onStart([]() {
            Serial.println("Start");
        });
        ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Web socket error[%u]\n", error);
            if (error == OTA_AUTH_ERROR) {
                Serial.println("Auth Failed");
            } else if (error == OTA_BEGIN_ERROR) {
                Serial.println("Begin Failed");
            } else if (error == OTA_CONNECT_ERROR) {
                Serial.println("Connect Failed");
            } else if (error == OTA_RECEIVE_ERROR) {
                Serial.println("Receive Failed");
            } else if (error == OTA_END_ERROR) {
                Serial.println("End Failed");
            } else {
                Serial.println("Other error");
            }
        });
        ArduinoOTA.begin();
    }

    void loop() {
        ArduinoOTA.handle();
    }
};

}}    // namespace farmhub::client
