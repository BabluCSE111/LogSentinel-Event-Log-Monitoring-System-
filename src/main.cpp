#include <iostream>
#include "LogReader.h"
#include "RuleEngine.h"

using namespace std;

int main() {

    LogReader reader;

    vector<Event> events = reader.readFile("logs/test.log");

    RuleEngine engine;

    for (Event event : events) {

        cout << "Event ID: " << event.getEventId() << endl;
        cout << "Timestamp: " << event.getTimestamp() << endl;
        cout << "Source: " << event.getSource() << endl;
        cout << "Username: " << event.getUsername() << endl;
        cout << "Message: " << event.getMessage() << endl;
        cout << "Severity: " << severityToString(event.getSeverity()) << endl;

        if (engine.isSuspicious(event)) {
            cout << "Status: SUSPICIOUS" << endl;
        }
        else {
            cout << "Status: NORMAL" << endl;
        }

        cout << "------------------------" << endl;
    }

    return 0;
}