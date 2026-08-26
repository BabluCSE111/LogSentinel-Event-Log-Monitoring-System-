#ifndef ALERT_H
#define ALERT_H

#include "Event.h"
#include <iostream>

using namespace std;

class Alert {
public:

    void showAlert(Event event) {

        cout << "================================" << endl;
        cout << "SECURITY ALERT" << endl;
        cout << "================================" << endl;

        cout << "Event ID: " << event.getEventId() << endl;
        cout << "Username: " << event.getUsername() << endl;
        cout << "Reason: " << event.getMessage() << endl;
        cout << "Severity: " << severityToString(event.getSeverity()) << endl;

        cout << "================================" << endl;
    }
};

#endif