#ifndef EVENT_H
#define EVENT_H

#include <string>

enum Severity {
    INFO,
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};

std::string severityToString(Severity severity);

Severity severityFromString(const std::string& text);

class Event
{
private:
    int eventId;
    std::string timestamp;
    std::string source;
    std::string username;
    std::string message;
    Severity severity;

public:
    Event(
        int eventId,
        std::string timestamp,
        std::string source,
        std::string username,
        std::string message,
        Severity severity
    );

    int getEventId() const;
    std::string getTimestamp() const;
    std::string getSource() const;
    std::string getUsername() const;
    std::string getMessage() const;
    Severity getSeverity() const;
};

#endif
