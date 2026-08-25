#include <iostream>
#include "Event.h"

using namespace std;

int main() {

    Event event(
        4625,
        "2026-08-25 19:00:00",
        "Windows Security",
        "Administrator",
        "Failed login attempt",
        HIGH
    );

    cout << "Event ID: " << event.getEventId() << endl;
    cout << "Timestamp: " << event.getTimestamp() << endl;
    cout << "Source: " << event.getSource() << endl;
    cout << "Username: " << event.getUsername() << endl;
    cout << "Message: " << event.getMessage() << endl;
    cout << "Severity: " << severityToString(event.getSeverity()) << endl;

    return 0;
}