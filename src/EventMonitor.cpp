
#include <windows.h>
#include <winevt.h>

#include "EventMonitor.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

static EventMonitor* activeMonitor = nullptr;

BOOL WINAPI consoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT ||
        signal == CTRL_BREAK_EVENT ||
        signal == CTRL_CLOSE_EVENT)
    {
        if (activeMonitor != nullptr)
        {
            activeMonitor->stop();
        }

        return TRUE;
    }

    return FALSE;
}

static std::wstring getXmlValue(
    const std::wstring& xml,
    const std::wstring& name
)
{
    std::wstring singleQuoteTag =
        L"<Data Name='" + name + L"'>";

    size_t start = xml.find(singleQuoteTag);

    if (start != std::wstring::npos)
    {
        start += singleQuoteTag.length();

        size_t end = xml.find(L"</Data>", start);

        if (end != std::wstring::npos)
        {
            return xml.substr(start, end - start);
        }
    }

    std::wstring doubleQuoteTag =
        L"<Data Name=\"" + name + L"\">";

    start = xml.find(doubleQuoteTag);

    if (start != std::wstring::npos)
    {
        start += doubleQuoteTag.length();

        size_t end = xml.find(L"</Data>", start);

        if (end != std::wstring::npos)
        {
            return xml.substr(start, end - start);
        }
    }

    return L"";
}

DWORD WINAPI eventCallback(
    EVT_SUBSCRIBE_NOTIFY_ACTION action,
    void* userContext,
    EVT_HANDLE event
)
{
    if (action != EvtSubscribeActionDeliver)
    {
        return 0;
    }

    EventMonitor* monitor =
        static_cast<EventMonitor*>(userContext);

    if (monitor != nullptr)
    {
        monitor->processEvent(event);
    }

    return 0;
}

EventMonitor::EventMonitor(
    std::function<void(Event)> handler,
    const Config& config
)
    : eventHandler(handler),
      config(config),
      subscription(nullptr),
      running(false)
{
}

void EventMonitor::processEvent(EVT_HANDLE event)
{
    DWORD bufferUsed = 0;
    DWORD propertyCount = 0;

    EvtRender(
        nullptr,
        event,
        EvtRenderEventXml,
        0,
        nullptr,
        &bufferUsed,
        &propertyCount
    );

    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::cout
            << "Could not get event size."
            << std::endl;

        EvtClose(event);
        return;
    }

    std::vector<wchar_t> buffer(
        bufferUsed / sizeof(wchar_t) + 1
    );

    if (!EvtRender(
            nullptr,
            event,
            EvtRenderEventXml,
            bufferUsed,
            buffer.data(),
            &bufferUsed,
            &propertyCount))
    {
        std::cout
            << "Could not render event."
            << std::endl;

        EvtClose(event);
        return;
    }

    std::wstring xml(buffer.data());

    int id = 0;

    size_t eventIdStart =
        xml.find(L"<EventID>");

    size_t eventIdEnd =
        xml.find(L"</EventID>");

    if (eventIdStart != std::wstring::npos &&
        eventIdEnd != std::wstring::npos &&
        eventIdEnd > eventIdStart)
    {
        eventIdStart +=
            std::wstring(L"<EventID>").length();

        std::wstring eventId =
            xml.substr(
                eventIdStart,
                eventIdEnd - eventIdStart
            );

        try
        {
            id = std::stoi(eventId);
        }
        catch (...)
        {
            id = 0;
        }
    }

    if (!config.hasRule(id))
    {
        EvtClose(event);
        return;
    }

    std::wstring username =
        getXmlValue(
            xml,
            L"TargetUserName"
        );

    if (username.empty())
    {
        username =
            getXmlValue(
                xml,
                L"SubjectUserName"
            );
    }

    std::wstring timestamp;

    size_t timeStart =
        xml.find(L"SystemTime='");

    if (timeStart != std::wstring::npos)
    {
        timeStart +=
            std::wstring(L"SystemTime='").length();

        size_t timeEnd =
            xml.find(L"'", timeStart);

        if (timeEnd != std::wstring::npos)
        {
            timestamp =
                xml.substr(
                    timeStart,
                    timeEnd - timeStart
                );
        }
    }
    else
    {
        timeStart =
            xml.find(L"SystemTime=\"");

        if (timeStart != std::wstring::npos)
        {
            timeStart +=
                std::wstring(L"SystemTime=\"").length();

            size_t timeEnd =
                xml.find(L"\"", timeStart);

            if (timeEnd != std::wstring::npos)
            {
                timestamp =
                    xml.substr(
                        timeStart,
                        timeEnd - timeStart
                    );
            }
        }
    }

    std::wstring ipAddress =
        getXmlValue(
            xml,
            L"IpAddress"
        );

    std::wstring source =
        L"Windows Security";

    Rule rule =
        config.getRule(id);

    Severity eventSeverity =
        rule.severity;

    std::string timestampString(
        timestamp.begin(),
        timestamp.end()
    );

    std::string sourceString(
        source.begin(),
        source.end()
    );

    std::string usernameString(
        username.begin(),
        username.end()
    );

    Event liveEvent(
        id,
        timestampString,
        sourceString,
        usernameString,
        rule.message,
        eventSeverity
    );

    std::wcout
        << L"\n========================================"
        << std::endl;

    std::wcout
        << L"          LOGSENTINEL LIVE EVENT"
        << std::endl;

    std::wcout
        << L"========================================"
        << std::endl;

    std::wcout
        << L"Event ID   : "
        << id
        << std::endl;

    std::wcout
        << L"Timestamp  : "
        << timestamp
        << std::endl;

    std::wcout
        << L"Username   : "
        << username
        << std::endl;

    std::wcout
        << L"IP Address : "
        << (ipAddress.empty() ? L"-" : ipAddress)
        << std::endl;

    std::wcout
        << L"Severity   : "
        << severityToString(eventSeverity).c_str()
        << std::endl;

    std::wcout
        << L"Status     : "
        << (rule.suspicious ? L"SUSPICIOUS" : L"NORMAL")
        << std::endl;

    std::wcout
        << L"Message    : "
        << rule.message.c_str()
        << std::endl;

    std::wcout
        << L"========================================"
        << std::endl;

    if (eventHandler)
    {
        eventHandler(liveEvent);
    }

    EvtClose(event);
}

void EventMonitor::start()
{
    if (running)
    {
        std::cout
            << "Event monitoring is already running."
            << std::endl;

        return;
    }

    std::cout
        << "\n========================================"
        << std::endl;

    std::cout
        << "      LOGSENTINEL SECURITY MONITOR"
        << std::endl;

    std::cout
        << "========================================"
        << std::endl;

    std::cout
        << "Starting Windows Event Log monitoring..."
        << std::endl;

    std::vector<int> eventIds =
        config.getEventIds();

    if (eventIds.empty())
    {
        std::cout
            << "No event IDs configured."
            << std::endl;

        return;
    }

    std::wstringstream queryStream;

    queryStream
        << L"*[System[(";

    for (size_t i = 0; i < eventIds.size(); ++i)
    {
        if (i > 0)
        {
            queryStream
                << L" or ";
        }

        queryStream
            << L"EventID="
            << eventIds[i];
    }

    queryStream
        << L")]]";

    std::wstring query =
        queryStream.str();

    std::wcout
        << L"Subscription query: "
        << query
        << std::endl;

    subscription =
        EvtSubscribe(
            nullptr,
            nullptr,
            L"Security",
            query.c_str(),
            nullptr,
            this,
            eventCallback,
            EvtSubscribeToFutureEvents
        );

    if (subscription == nullptr)
    {
        DWORD error =
            GetLastError();

        std::cout
            << "Could not subscribe to Windows Security Event Log."
            << std::endl;

        std::cout
            << "Windows Error Code: "
            << error
            << std::endl;

        return;
    }

    running = true;
    activeMonitor = this;

    SetConsoleCtrlHandler(
        consoleHandler,
        TRUE
    );

    std::cout
        << "Monitoring Windows Security Event Log..."
        << std::endl;

    std::cout
        << "Waiting for security events..."
        << std::endl;

    std::cout
        << "Press Ctrl+C to stop monitoring."
        << std::endl;

    while (running)
    {
        Sleep(1000);
    }

    SetConsoleCtrlHandler(
        consoleHandler,
        FALSE
    );

    activeMonitor = nullptr;

    if (subscription != nullptr)
    {
        EvtClose(subscription);
        subscription = nullptr;
    }

    std::cout
        << "Windows Event Log monitoring stopped."
        << std::endl;
}

void EventMonitor::stop()
{
    if (!running)
    {
        return;
    }

    std::cout
        << "\nStopping LogSentinel..."
        << std::endl;

    running = false;
}

bool EventMonitor::isRunning() const
{
    return running;
}
