#ifndef LOGREADER_H
#define LOGREADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Event.h"

class LogReader {
public:

    std::vector<Event> readFile(const std::string& filePath)
    {
        std::vector<Event> events;

        std::ifstream file(filePath);

        if (!file.is_open())
        {
            std::cout
                << "Error: Could not open log file: "
                << filePath
                << std::endl;

            return events;
        }

        std::string line;

        while (std::getline(file, line))
        {
            std::stringstream ss(line);

            std::string timestamp;
            std::string eventId;
            std::string source;
            std::string username;
            std::string message;
            std::string severity;

            std::getline(ss, timestamp, '|');
            std::getline(ss, eventId, '|');
            std::getline(ss, source, '|');
            std::getline(ss, username, '|');
            std::getline(ss, message, '|');
            std::getline(ss, severity, '|');

            int id = std::stoi(eventId);

            Severity eventSeverity;

            if (severity == "INFO")
            {
                eventSeverity = INFO;
            }
            else if (severity == "LOW")
            {
                eventSeverity = LOW;
            }
            else if (severity == "MEDIUM")
            {
                eventSeverity = MEDIUM;
            }
            else if (severity == "HIGH")
            {
                eventSeverity = HIGH;
            }
            else
            {
                eventSeverity = CRITICAL;
            }

            Event event(
                id,
                timestamp,
                source,
                username,
                message,
                eventSeverity
            );

            events.push_back(event);
        }

        return events;
    }
};

#endif
