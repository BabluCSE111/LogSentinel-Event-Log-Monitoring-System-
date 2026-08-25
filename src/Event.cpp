#include "Event.h"

Event::Event(
    int eventId,
    string timestamp,
    string source,
    string username,
    string message,
    Severity severity
) {
    this->eventId = eventId;
    this->timestamp = timestamp;
    this->source = source;
    this->username = username;
    this->message = message;
    this->severity = severity;
}

int Event::getEventId() const {
    return eventId;
}

string Event::getTimestamp() const {
    return timestamp;
}

string Event::getSource() const {
    return source;
}

string Event::getUsername() const {
    return username;
}

string Event::getMessage() const {
    return message;
}

Severity Event::getSeverity() const {
    return severity;
}

string severityToString(Severity severity) {
    switch (severity) {
        case INFO:
            return "INFO";

        case LOW:
            return "LOW";

        case MEDIUM:
            return "MEDIUM";

        case HIGH:
            return "HIGH";

        case CRITICAL:
            return "CRITICAL";
    }

    return "UNKNOWN";
}