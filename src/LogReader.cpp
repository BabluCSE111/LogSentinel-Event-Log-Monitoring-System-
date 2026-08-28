#include "LogReader.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::vector<Event> LogReader::readFile(
    const std::string& filePath
)
{
    std::vector<Event> events;

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout
            << "Error: Could not open log file: "
            << filePath
            << std::endl;

        return events;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);

        std::string timestamp;
        std::string eventIdText;
        std::string source;
        std::string username;
        std::string message;
        std::string severityText;

        if (!std::getline(ss, timestamp, '|') ||
            !std::getline(ss, eventIdText, '|') ||
            !std::getline(ss, source, '|') ||
            !std::getline(ss, username, '|') ||
            !std::getline(ss, message, '|') ||
            !std::getline(ss, severityText, '|'))
        {
            continue;
        }

        int eventId;

        try
        {
            size_t position = 0;

            eventId =
                std::stoi(
                    eventIdText,
                    &position
                );

            if (position != eventIdText.length())
            {
                continue;
            }
        }
        catch (...)
        {
            continue;
        }

        Severity severity;

        if (severityText == "INFO")
        {
            severity = INFO;
        }
        else if (severityText == "LOW")
        {
            severity = LOW;
        }
        else if (severityText == "MEDIUM")
        {
            severity = MEDIUM;
        }
        else if (severityText == "HIGH")
        {
            severity = HIGH;
        }
        else if (severityText == "CRITICAL")
        {
            severity = CRITICAL;
        }
        else
        {
            continue;
        }

        if (timestamp.empty() ||
            source.empty() ||
            username.empty() ||
            message.empty())
        {
            continue;
        }

        Event event(
            eventId,
            timestamp,
            source,
            username,
            message,
            severity
        );

        events.push_back(event);
    }

    return events;
}
