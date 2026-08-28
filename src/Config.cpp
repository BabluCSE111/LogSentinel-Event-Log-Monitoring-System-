#include "Config.h"

#include <fstream>
#include <iostream>

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

    int eventId;
    std::string severityText;
    std::string suspiciousText;

    while (file >> eventId >> severityText >> suspiciousText)
    {
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

        bool suspicious =
            suspiciousText == "SUSPICIOUS";

        eventRules[eventId] =
            {severity, suspicious};
    }

    valid = !eventRules.empty();
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
