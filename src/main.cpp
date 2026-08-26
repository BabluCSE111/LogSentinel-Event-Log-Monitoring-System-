#include <iostream>
#include "LogReader.h"

using namespace std;

int main() {

    LogReader reader;

    vector<Event> events = reader.readFile("logs/test.log");

    for (Event event : events) {

        cout << "Event ID: " << event.getEventId() << endl;
        cout << "Timestamp: " << event.getTimestamp() << endl;
        cout << "Source: " << event.getSource() << endl;
        cout << "Username: " << event.getUsername() << endl;
        cout << "Message: " << event.getMessage() << endl;
        cout << "Severity: " << severityToString(event.getSeverity()) << endl;

        cout << "------------------------" << endl;
    }

    return 0;
}