#include <iostream>
#include "LogReader.h"
#include "RuleEngine.h"
#include "Alert.h"
#include "AlertManager.h"
#include "EventMonitor.h"

using namespace std;

int main() {

    LogReader reader;

    vector<Event> events = reader.readFile("logs/test.log");

    RuleEngine engine;

    Alert alert;

    AlertManager alertManager;

    EventMonitor monitor([&](Event event) {

        cout << "Real event received: " << event.getEventId() << endl;

        if (engine.isSuspicious(event)) {
            cout << "LIVE EVENT: SUSPICIOUS" << endl;
            alert.showAlert(event);
            alertManager.addAlert(event);
        }
        else {
            cout << "LIVE EVENT: NORMAL" << endl;
        }
    });

    for (Event event : events) {

        cout << "Event ID: " << event.getEventId() << endl;
        cout << "Timestamp: " << event.getTimestamp() << endl;
        cout << "Source: " << event.getSource() << endl;
        cout << "Username: " << event.getUsername() << endl;
        cout << "Message: " << event.getMessage() << endl;
        cout << "Severity: " << severityToString(event.getSeverity()) << endl;

        if (engine.isSuspicious(event)) {

            cout << "Status: SUSPICIOUS" << endl;

            alert.showAlert(event);

            alertManager.addAlert(event);
        }
        else {

            cout << "Status: NORMAL" << endl;
        }

        cout << "------------------------" << endl;
    }

    cout << endl;
    cout << "Total Alerts: "
         << alertManager.getAlertCount()
         << endl;

    monitor.start();

    return 0;
}