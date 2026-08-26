#ifndef ALERTMANAGER_H
#define ALERTMANAGER_H

#include "Alert.h"
#include "Event.h"
#include <vector>

using namespace std;

class AlertManager {
private:

    vector<Event> alerts;

public:

    void addAlert(Event event) {

        alerts.push_back(event);
    }

    int getAlertCount() {

        return alerts.size();
    }

    void showAlerts() {

        for (Event event : alerts) {

            Alert alert;

            alert.showAlert(event);
        }
    }
};

#endif