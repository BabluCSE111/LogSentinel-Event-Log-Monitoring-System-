#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>

bool Config::isValidSeverity(
    const std::string& value
) const
{
    return value == "INFO" ||
           value == "LOW" ||
           value == "MEDIUM" ||
           value == "HIGH" ||
           value == "CRITICAL";
}

Config::Config()
    : valid(false)
{
    std::ifstream file("config/rules.conf");

    if (!file)
    {
        std::cerr
            << "ERROR: Could not open config/rules.conf"
            << std::endl;

        return;
    }

    std::string line;
    int lineNumber = 0;
    bool hasInvalidRule = false;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (line.empty())
        {
            continue;
        }

        std::stringstream stream(line);

        int eventId;
        std::string severityText;
        std::string suspiciousText;
        std::string extra;

        if (!(stream >> eventId >>
              severityText >>
              suspiciousText) ||
            (stream >> extra))
        {
            std::cerr
                << "ERROR: Invalid configuration at line "
                << lineNumber
                << std::endl;

            hasInvalidRule = true;
            continue;
        }

        if (eventId <= 0)
        {
            std::cerr
                << "ERROR: Invalid Event ID at line "
                << lineNumber
                << std::endl;

            hasInvalidRule = true;
            continue;
        }

        if (eventRules.find(eventId) != eventRules.end())
        {
            std::cerr
                << "ERROR: Duplicate Event ID "
                << eventId
                << " at line "
                << lineNumber
                << std::endl;

            hasInvalidRule = true;
            continue;
        }

        if (!isValidSeverity(severityText))
        {
            std::cerr
                << "ERROR: Invalid severity for Event ID "
                << eventId
                << std::endl;

            hasInvalidRule = true;
            continue;
        }

        if (suspiciousText != "SUSPICIOUS" &&
            suspiciousText != "NORMAL")
        {
            std::cerr
                << "ERROR: Invalid rule type for Event ID "
                << eventId
                << std::endl;

            hasInvalidRule = true;
            continue;
        }

        Severity severity = INFO;

        if (severityText == "LOW")
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
            {severity, suspicious};
    }

    if (eventRules.empty())
    {
        valid = false;
        return;
    }

    valid = !hasInvalidRule;
}

Rule Config::getRule(int eventId) const
{
    auto it = eventRules.find(eventId);

    if (it != eventRules.end())
    {
        return it->second;
    }

    return {INFO, false};
}

bool Config::hasRule(int eventId) const
{
    return eventRules.find(eventId) != eventRules.end();
}

bool Config::isValid() const
{
    return valid;
}
