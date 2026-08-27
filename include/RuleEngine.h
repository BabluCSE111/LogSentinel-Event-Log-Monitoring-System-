#ifndef RULEENGINE_H
#define RULEENGINE_H

#include "Event.h"
#include "Config.h"

class RuleEngine
{
private:
    Config config;

public:
    bool isSuspicious(const Event& event)
    {
        Rule rule =
            config.getRule(event.getEventId());

        return rule.suspicious;
    }
};

#endif