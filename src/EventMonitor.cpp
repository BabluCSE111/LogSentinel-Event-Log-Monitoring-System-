#include <windows.h>
#include <winevt.h>

#include "EventMonitor.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional>

std::wstring getXmlValue(
    const std::wstring& xml,
    const std::wstring& name
)
{
    std::wstring tag =
        L"<Data Name='" + name + L"'>";

    size_t start =
        xml.find(tag);

    if (start == std::wstring::npos)
        return L"";

    start += tag.length();

    size_t end =
        xml.find(L"</Data>", start);

    if (end == std::wstring::npos)
        return L"";

    return xml.substr(
        start,
        end - start
    );
}

DWORD WINAPI eventCallback(
    EVT_SUBSCRIBE_NOTIFY_ACTION action,
    void* userContext,
    EVT_HANDLE event
)
{
    if (action != EvtSubscribeActionDeliver)
        return 0;

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
      config(config)
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

    std::wstring xml(
        buffer.data()
    );

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

    std::wstring message;

    if (id == 4625)
    {
        message =
            L"Failed login attempt";
    }
    else if (id == 4624)
    {
        message =
            L"Successful login";
    }
    else if (id == 4720)
    {
        message =
            L"New user account created";
    }
    else if (id == 4672)
    {
        message =
            L"Special privileges assigned";
    }
    else
    {
        message =
            L"Windows Security Event";
    }

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

    std::string messageString(
        message.begin(),
        message.end()
    );

    Event liveEvent(
        id,
        timestampString,
        sourceString,
        usernameString,
        messageString,
        eventSeverity
    );

    std::wcout
        << L"\n========== WINDOWS EVENT ==========\n";

    std::wcout
        << L"Event ID: "
        << id
        << std::endl;

    std::wcout
        << L"Timestamp: "
        << timestamp
        << std::endl;

    std::wcout
        << L"Username: "
        << username
        << std::endl;

    std::wcout
        << L"IP Address: "
        << ipAddress
        << std::endl;

    std::wcout
        << L"===================================\n";

    if (eventHandler)
    {
        eventHandler(liveEvent);
    }

    EvtClose(event);
}

void EventMonitor::start()
{
    std::cout
        << "Starting Windows Event Log monitoring..."
        << std::endl;

    EVT_HANDLE subscription =
        EvtSubscribe(
            nullptr,
            nullptr,
            L"Security",

            L"*[System["
            L"(EventID=4625 or "
            L"EventID=4624 or "
            L"EventID=4720 or "
            L"EventID=4672)"
            L"]]",

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

    std::cout
        << "Monitoring Windows Security Event Log..."
        << std::endl;

    std::cout
        << "Waiting for security events..."
        << std::endl;

    while (true)
    {
        Sleep(1000);
    }

    EvtClose(subscription);
}