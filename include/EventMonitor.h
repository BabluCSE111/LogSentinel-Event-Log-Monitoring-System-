#ifndef EVENTMONITOR_H
#define EVENTMONITOR_H

#include <windows.h>
#include <winevt.h>
#include <functional>

#include "Event.h"
#include "Config.h"

class EventMonitor
{
private:
    std::function<void(Event)> eventHandler;
    const Config& config;

    EVT_HANDLE subscription;
    bool running;

    void processEvent(EVT_HANDLE event);

    friend DWORD WINAPI eventCallback(
        EVT_SUBSCRIBE_NOTIFY_ACTION action,
        void* userContext,
        EVT_HANDLE event
    );

public:
    EventMonitor(
        std::function<void(Event)> handler,
        const Config& config
    );

    void start();

    void stop();

    bool isRunning() const;
};

#endif
