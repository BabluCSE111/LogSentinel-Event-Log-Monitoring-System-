#ifndef ALERT_H
#define ALERT_H

#include <iostream>

#include "Event.h"

class Alert
{
public:

    void showAlert(const Event& event)
    {
        std::cout
            << "================================"
            << std::endl;

        if (event.getSeverity() == MEDIUM)
        {
            std::cout
                << "SECURITY WARNING"
                << std::endl;
        }
        else if (event.getSeverity() == HIGH)
        {
            std::cout
                << "SECURITY ALERT"
                << std::endl;
        }
        else if (event.getSeverity() == CRITICAL)
        {
            std::cout
                << "CRITICAL SECURITY ALERT"
                << std::endl;
        }
        else
        {
            std::cout
                << "SECURITY NOTICE"
                << std::endl;
        }

        std::cout
            << "================================"
            << std::endl;

        std::cout
            << "Event ID: "
            << event.getEventId()
            << std::endl;

        std::cout
            << "Username: "
            << event.getUsername()
            << std::endl;

        std::cout
            << "Reason: "
            << event.getMessage()
            << std::endl;

        std::cout
            << "Severity: "
            << severityToString(event.getSeverity())
            << std::endl;

        std::cout
            << "================================"
            << std::endl;
    }
};

#endif
