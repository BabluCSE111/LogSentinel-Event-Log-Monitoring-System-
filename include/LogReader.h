#ifndef LOGREADER_H
#define LOGREADER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Event.h"

using namespace std;

class LogReader {
public:

    vector<Event> readFile(string filePath) {

        vector<Event> events;

        ifstream file(filePath);

        if (!file.is_open()) {
            cout << "Error: Could not open log file: " << filePath << endl;
            return events;
        }

        string line;

        while (getline(file, line)) {

            stringstream ss(line);

            string timestamp;
            string eventId;
            string source;
            string username;
            string message;
            string severity;

            getline(ss, timestamp, '|');
            getline(ss, eventId, '|');
            getline(ss, source, '|');
            getline(ss, username, '|');
            getline(ss, message, '|');
            getline(ss, severity, '|');

            int id = stoi(eventId);

            Severity eventSeverity;

            if (severity == "INFO") {
                eventSeverity = INFO;
            }
            else if (severity == "LOW") {
                eventSeverity = LOW;
            }
            else if (severity == "MEDIUM") {
                eventSeverity = MEDIUM;
            }
            else if (severity == "HIGH") {
                eventSeverity = HIGH;
            }
            else {
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