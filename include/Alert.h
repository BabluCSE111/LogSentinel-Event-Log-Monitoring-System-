#ifndef ALERT_H
#define ALERT_H
#include <iostream>

#include "Event.h"
#include <iostream>


class Alert {
public:

    void showAlert(Event event) {

        std::cout << "================================" << std::endl;
        std::cout << "SECURITY ALERT" << std::endl;
        std::cout << "================================" << std::endl;

        std::cout << "Event ID: " << event.getEventId() << std::endl;
        std::cout << "Username: " << event.getUsername() << std::endl;
        std::cout << "Reason: " << event.getMessage() << std::endl;
        std::cout << "Severity: " << severityToString(event.getSeverity()) << std::endl;

        std::cout << "================================" << std::endl;
    }
};

#endif