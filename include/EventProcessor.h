#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "Event.h"
#include "RuleEngine.h"
#include "AlertManager.h"
#include "EventCorrelator.h"

class EventProcessor
{
private:
    RuleEngine& ruleEngine;
    AlertManager& alertManager;
    EventCorrelator& correlator;

public:
    EventProcessor(
        RuleEngine& ruleEngine,
        AlertManager& alertManager,
        EventCorrelator& correlator
    );

    void process(const Event& event);
};

#endif
