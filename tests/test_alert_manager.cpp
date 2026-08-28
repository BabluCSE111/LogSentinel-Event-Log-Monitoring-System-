#include <cassert>
#include <iostream>
#include <cstdio>

#include "Event.h"
#include "AlertManager.h"

int main()
{
    std::remove("logs/alerts.log");

    AlertManager manager;

    Event failedLogin(
        4625,
        "2026-08-28 15:00:00",
        "Windows Security",
        "Administrator",
        "Failed login attempt",
        HIGH
    );

    Event newUser(
        4720,
        "2026-08-28 15:10:00",
        "Windows Security",
        "Guest",
        "New user account created",
        MEDIUM
    );

    manager.addAlert(failedLogin);
    manager.addAlert(newUser);

    assert(manager.getAlertCount() == 2);
    assert(manager.getSeverityCount(HIGH) == 1);
    assert(manager.getSeverityCount(MEDIUM) == 1);
    assert(manager.getSeverityCount(INFO) == 0);

    manager.addAlert(failedLogin);

    assert(manager.getAlertCount() == 2);
    assert(manager.getSeverityCount(HIGH) == 1);

    std::cout << "AlertManager tests passed."
              << std::endl;

    return 0;
}
