#ifndef EVENT_H
#define EVENT_H

#include <string>

using namespace std;

enum Severity {
    INFO,
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};

string severityToString(Severity severity);

class Event {
private:
    int eventId;
    string timestamp;
    string source;
    string username;
    string message;
    Severity severity;

public:
    Event(
        int eventId,
        string timestamp,
        string source,
        string username,
        string message,
        Severity severity
    );

    int getEventId() const;
    string getTimestamp() const;
    string getSource() const;
    string getUsername() const;
    string getMessage() const;
    Severity getSeverity() const;
};

#endif