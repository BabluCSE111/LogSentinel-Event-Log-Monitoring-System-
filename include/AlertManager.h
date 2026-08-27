#ifndef ALERTMANAGER_H
#define ALERTMANAGER_H

#include <vector>
#include <fstream>

#include "Alert.h"
#include "Event.h"

class AlertManager
{
private:
    std::vector<Event> alerts;

public:

    void addAlert(Event event)
    {
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

    int getAlertCount()
    {
        return alerts.size();
    }

    void showAlerts()
    {
        for (Event event : alerts)
        {
            Alert alert;
            alert.showAlert(event);
        }
    }
};

#endif