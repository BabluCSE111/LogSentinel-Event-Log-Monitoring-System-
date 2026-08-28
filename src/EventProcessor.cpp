#include "EventProcessor.h"

#include <iostream>

EventProcessor::EventProcessor(
    RuleEngine& ruleEngine,
    AlertManager& alertManager,
    EventCorrelator& correlator
)
    : ruleEngine(ruleEngine),
      alertManager(alertManager),
      correlator(correlator)
{
}

void EventProcessor::process(const Event& event)
{
    std::cout
        << "Event ID: "
        << event.getEventId()
        << std::endl;

    std::cout
        << "Timestamp: "
        << event.getTimestamp()
        << std::endl;

    std::cout
        << "Source: "
        << event.getSource()
        << std::endl;

    std::cout
        << "Username: "
        << event.getUsername()
        << std::endl;

    std::cout
        << "Message: "
        << event.getMessage()
        << std::endl;

    std::cout
        << "Severity: "
        << severityToString(event.getSeverity())
        << std::endl;

    bool suspicious =
        ruleEngine.isSuspicious(event);

    bool correlated =
        correlator.recordEvent(event);

    if (suspicious)
    {
        std::cout
            << "Status: SUSPICIOUS"
            << std::endl;

        alertManager.addAlert(event);
    }
    else
    {
        std::cout
            << "Status: NORMAL"
            << std::endl;
    }

    if (correlated)
    {
        Event correlationAlert(
            event.getEventId(),
            event.getTimestamp(),
            event.getSource(),
            event.getUsername(),
            "Multiple failed login attempts detected",
            CRITICAL
        );

        alertManager.addAlert(correlationAlert);

        std::cout
            << "CORRELATION ALERT: "
            << event.getUsername()
            << " has reached "
            << correlator.getFailedLoginCount(
                   event.getUsername()
               )
            << " failed login attempts."
            << std::endl;
    }

    std::cout
        << "------------------------"
        << std::endl;
}
