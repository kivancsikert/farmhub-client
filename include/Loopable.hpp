#pragma once

#include <chrono>

using namespace std::chrono;

namespace farmhub { namespace client {

template <class T>
class Loopable {
public:
    virtual T loop() = 0;
};

template <class T>
class TimedLoopable
    : public Loopable<T> {
public:
    T loop() override {
        auto currentTime = system_clock::now();
        if (currentTime - previousLoop > nextLoopAfter()) {
            previousLoop = currentTime;
            return timedLoop();
        }
        return defaultValue();
    }

    time_point<system_clock> getPreviousLoop() const {
        return previousLoop;
    }

protected:
    virtual T timedLoop() = 0;
    virtual T defaultValue() = 0;
    virtual milliseconds nextLoopAfter() const = 0;

private:
    time_point<system_clock> previousLoop;
};

}}    // namespace farmhub::client
