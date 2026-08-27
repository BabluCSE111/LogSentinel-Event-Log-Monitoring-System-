#include "Config.h"

Config::Config()
{
    eventRules[4624] = {INFO, false};
    eventRules[4625] = {HIGH, true};
    eventRules[4720] = {MEDIUM, true};
    eventRules[4672] = {INFO, false};
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