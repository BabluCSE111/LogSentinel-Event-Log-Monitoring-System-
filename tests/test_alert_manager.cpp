#include <cassert>
#include <fstream>
#include <iostream>
#include <cstdio>

#include "AlertManager.h"

int main()
{
    const std::string logFile = "logs/alerts.log";

    {
        std::ofstream file(logFile);

        file << "Event ID: 4625\n";
        file << "Timestamp: 2026-08-28 15:00:00\n";
        file << "Username: Administrator\n";
        file << "Reason: Failed login attempt\n";
        file << "Severity: HIGH\n";
        file << "--------------------------------\n";

        file << "Event ID: 4720\n";
        file << "Timestamp: 2026-08-28 15:10:00\n";
        file << "Username: Guest\n";
        file << "Reason: New user account created\n";
        file << "Severity: MEDIUM\n";
        file << "--------------------------------\n";
    }

    AlertManager manager;

    manager.loadAlerts();

    assert(manager.getAlertCount() == 2);
    assert(manager.getSeverityCount(HIGH) == 1);
    assert(manager.getSeverityCount(MEDIUM) == 1);
    assert(manager.getSeverityCount(INFO) == 0);

    const std::vector<Event>& alerts =
        manager.getAlerts();

    assert(alerts[0].getEventId() == 4625);
    assert(alerts[0].getUsername() == "Administrator");
    assert(alerts[0].getSeverity() == HIGH);

    assert(alerts[1].getEventId() == 4720);
    assert(alerts[1].getUsername() == "Guest");
    assert(alerts[1].getSeverity() == MEDIUM);

    Event duplicate(
        4625,
        "2026-08-28 15:00:00",
        "Windows Security",
        "Administrator",
        "Failed login attempt",
        HIGH
    );

    manager.addAlert(duplicate);

    assert(manager.getAlertCount() == 2);

    Event newAlert(
        4672,
        "2026-08-28 15:20:00",
        "Windows Security",
        "Administrator",
        "Special privileges assigned",
        CRITICAL
    );

    manager.addAlert(newAlert);

    assert(manager.getAlertCount() == 3);
    assert(manager.getSeverityCount(CRITICAL) == 1);

    AlertManager reloadedManager;

    reloadedManager.loadAlerts();

    assert(reloadedManager.getAlertCount() == 3);
    assert(reloadedManager.getSeverityCount(HIGH) == 1);
    assert(reloadedManager.getSeverityCount(MEDIUM) == 1);
    assert(reloadedManager.getSeverityCount(CRITICAL) == 1);

    const std::vector<Event>& reloadedAlerts =
        reloadedManager.getAlerts();

    assert(reloadedAlerts[2].getEventId() == 4672);
    assert(reloadedAlerts[2].getUsername() == "Administrator");
    assert(reloadedAlerts[2].getSeverity() == CRITICAL);

    std::remove(logFile.c_str());

    std::cout
        << "AlertManager persistence tests passed."
        << std::endl;

    return 0;
}
