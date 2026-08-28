#ifndef RULEENGINE_H
#define RULEENGINE_H

#include "Event.h"
#include "Config.h"

class RuleEngine
{
private:
    const Config& config;

public:
    RuleEngine(const Config& config)
        : config(config)
    {
    }

    bool isSuspicious(const Event& event) const
    {
        Rule rule =
            config.getRule(event.getEventId());

        return rule.suspicious;
    }
};

#endif
