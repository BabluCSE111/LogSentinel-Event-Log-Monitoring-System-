#include "Event.h"

Event::Event(
    int eventId,
    std::string timestamp,
    std::string source,
    std::string username,
    std::string message,
    Severity severity
)
    : eventId(eventId),
      timestamp(timestamp),
      source(source),
      username(username),
      message(message),
      severity(severity)
{
}

int Event::getEventId() const
{
    return eventId;
}

std::string Event::getTimestamp() const
{
    return timestamp;
}

std::string Event::getSource() const
{
    return source;
}

std::string Event::getUsername() const
{
    return username;
}

std::string Event::getMessage() const
{
    return message;
}

Severity Event::getSeverity() const
{
    return severity;
}

std::string severityToString(Severity severity)
{
    switch (severity)
    {
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

        default:
            return "UNKNOWN";
    }
}
