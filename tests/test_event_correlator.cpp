#include <cassert>
#include <iostream>

#include "Event.h"
#include "EventCorrelator.h"

int main()
{
    EventCorrelator correlator;

    Event failedLogin1(
        4625,
        "2026-08-28 16:00:00",
        "Windows Security",
        "Hp",
        "Failed login attempt",
        HIGH
    );

    Event failedLogin2(
        4625,
        "2026-08-28 16:01:00",
        "Windows Security",
        "Hp",
        "Failed login attempt",
        HIGH
    );

    Event failedLogin3(
        4625,
        "2026-08-28 16:02:00",
        "Windows Security",
        "Hp",
        "Failed login attempt",
        HIGH
    );

    assert(correlator.recordEvent(failedLogin1) == false);
    assert(correlator.getFailedLoginCount("Hp") == 1);

    assert(correlator.recordEvent(failedLogin2) == false);
    assert(correlator.getFailedLoginCount("Hp") == 2);

    assert(correlator.recordEvent(failedLogin3) == true);
    assert(correlator.getFailedLoginCount("Hp") == 3);

    Event successfulLogin(
        4624,
        "2026-08-28 16:03:00",
        "Windows Security",
        "Hp",
        "Successful login",
        INFO
    );

    assert(correlator.recordEvent(successfulLogin) == false);
    assert(correlator.getFailedLoginCount("Hp") == 3);

    correlator.reset("Hp");

    assert(correlator.getFailedLoginCount("Hp") == 0);

    std::cout
        << "EventCorrelator tests passed."
        << std::endl;

    return 0;
}
