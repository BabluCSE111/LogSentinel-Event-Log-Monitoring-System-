#include <cassert>
#include <iostream>

#include "Event.h"
#include "Config.h"
#include "RuleEngine.h"
#include "AlertManager.h"
#include "EventCorrelator.h"
#include "EventProcessor.h"

int main()
{
    Config config;

    assert(config.isValid());

    RuleEngine engine(config);

    AlertManager alertManager;

    EventCorrelator correlator;

    EventProcessor processor(
        engine,
        alertManager,
        correlator
    );

    Event failedLogin1(
        4625,
        "2026-08-28 16:00:00",
        "Windows Security",
        "Administrator",
        "Failed login attempt",
        HIGH
    );

    Event failedLogin2(
        4625,
        "2026-08-28 16:01:00",
        "Windows Security",
        "Administrator",
        "Failed login attempt",
        HIGH
    );

    Event failedLogin3(
        4625,
        "2026-08-28 16:02:00",
        "Windows Security",
        "Administrator",
        "Failed login attempt",
        HIGH
    );

    Event failedLogin4(
        4625,
        "2026-08-28 16:03:00",
        "Windows Security",
        "Administrator",
        "Failed login attempt",
        HIGH
    );

    processor.process(failedLogin1);

    assert(alertManager.getAlertCount() == 1);
    assert(correlator.getFailedLoginCount("Administrator") == 1);

    processor.process(failedLogin2);

    assert(alertManager.getAlertCount() == 2);
    assert(correlator.getFailedLoginCount("Administrator") == 2);

    processor.process(failedLogin3);

    assert(alertManager.getAlertCount() == 4);
    assert(correlator.getFailedLoginCount("Administrator") == 3);

    processor.process(failedLogin4);

    assert(alertManager.getAlertCount() == 5);
    assert(correlator.getFailedLoginCount("Administrator") == 4);

    Event successfulLogin(
        4624,
        "2026-08-28 16:05:00",
        "Windows Security",
        "Administrator",
        "Successful login",
        INFO
    );

    processor.process(successfulLogin);

    assert(alertManager.getAlertCount() == 5);
    assert(correlator.getFailedLoginCount("Administrator") == 4);

    std::cout
        << "EventProcessor tests passed."
        << std::endl;

    return 0;
}