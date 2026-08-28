#ifndef ALERTMANAGER_H
#define ALERTMANAGER_H

#include <vector>
#include <fstream>
#include <iostream>

#include "Alert.h"
#include "Event.h"

class AlertManager
{
private:
    std::vector<Event> alerts;

    bool isDuplicate(const Event& event)
    {
        for (const Event& alert : alerts)
        {
            if (alert.getEventId() == event.getEventId() &&
                alert.getTimestamp() == event.getTimestamp() &&
                alert.getUsername() == event.getUsername() &&
                alert.getMessage() == event.getMessage())
            {
                return true;
            }
        }

        return false;
    }

public:

    void addAlert(const Event& event)
    {
        if (isDuplicate(event))
        {
            return;
        }

        alerts.push_back(event);

        std::ofstream file("logs/alerts.log", std::ios::app);

        if (file)
        {
            file << "Event ID: "
                 << event.getEventId()
                 << std::endl;

            file << "Timestamp: "
                 << event.getTimestamp()
                 << std::endl;

            file << "Username: "
                 << event.getUsername()
                 << std::endl;

            file << "Reason: "
                 << event.getMessage()
                 << std::endl;

            file << "Severity: "
                 << severityToString(event.getSeverity())
                 << std::endl;

            file << "--------------------------------"
                 << std::endl;
        }
    }

    int getAlertCount() const
    {
        return static_cast<int>(alerts.size());
    }

    int getSeverityCount(Severity severity) const
    {
        int count = 0;

        for (const Event& event : alerts)
        {
            if (event.getSeverity() == severity)
            {
                count++;
            }
        }

        return count;
    }

    void showStatistics() const
    {
        std::cout << "\n================================" << std::endl;
        std::cout << "LOGSENTINEL ALERT SUMMARY" << std::endl;
        std::cout << "================================" << std::endl;

        std::cout << "Total Alerts: "
                  << getAlertCount()
                  << std::endl;

        std::cout << "INFO: "
                  << getSeverityCount(INFO)
                  << std::endl;

        std::cout << "LOW: "
                  << getSeverityCount(LOW)
                  << std::endl;

        std::cout << "MEDIUM: "
                  << getSeverityCount(MEDIUM)
                  << std::endl;

        std::cout << "HIGH: "
                  << getSeverityCount(HIGH)
                  << std::endl;

        std::cout << "CRITICAL: "
                  << getSeverityCount(CRITICAL)
                  << std::endl;

        std::cout << "================================"
                  << std::endl;
    }

    void showAlerts()
    {
        for (const Event& event : alerts)
        {
            Alert alert;
            alert.showAlert(event);
        }
    }
};

#endif