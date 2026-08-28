#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>

Config::Config()
    : Config("config/rules.conf")
{
}

Config::Config(const std::string& filePath)
    : valid(false)
{
    std::ifstream file(filePath);

    if (!file)
    {
        std::cerr
            << "ERROR: Could not open "
            << filePath
            << std::endl;

        return;
    }

    int eventId;
    std::string severityText;
    std::string suspiciousText;
    std::string message;

    while (std::getline(file, message))
    {
        if (message.empty())
        {
            continue;
        }

        std::stringstream line(message);

        line >> eventId
             >> severityText
             >> suspiciousText;

        std::getline(line, message);

        if (!message.empty() &&
            message.front() == ' ')
        {
            message.erase(0, 1);
        }

        if (eventId <= 0)
        {
            std::cerr
                << "ERROR: Invalid Event ID "
                << eventId
                << std::endl;

            continue;
        }

        if (!isValidSeverity(severityText))
        {
            std::cerr
                << "ERROR: Invalid severity for Event ID "
                << eventId
                << std::endl;

            continue;
        }

        if (suspiciousText != "SUSPICIOUS" &&
            suspiciousText != "NORMAL")
        {
            std::cerr
                << "ERROR: Invalid rule type for Event ID "
                << eventId
                << std::endl;

            continue;
        }

        if (message.empty())
        {
            std::cerr
                << "ERROR: Missing message for Event ID "
                << eventId
                << std::endl;

            continue;
        }

        Severity severity = INFO;

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

        bool suspicious =
            suspiciousText == "SUSPICIOUS";

        eventRules[eventId] =
            {severity, suspicious, message};
    }

    valid = !eventRules.empty();
}

bool Config::isValidSeverity(
    const std::string& value) const
{
    return value == "INFO" ||
           value == "LOW" ||
           value == "MEDIUM" ||
           value == "HIGH" ||
           value == "CRITICAL";
}

Rule Config::getRule(int eventId) const
{
    auto it = eventRules.find(eventId);

    if (it != eventRules.end())
    {
        return it->second;
    }

    return {
        INFO,
        false,
        "Unknown Windows Security Event"
    };
}

bool Config::hasRule(int eventId) const
{
    return eventRules.find(eventId) != eventRules.end();
}

std::vector<int> Config::getEventIds() const
{
    std::vector<int> eventIds;

    for (const auto& entry : eventRules)
    {
        eventIds.push_back(entry.first);
    }

    return eventIds;
}

bool Config::isValid() const
{
    return valid;
}
