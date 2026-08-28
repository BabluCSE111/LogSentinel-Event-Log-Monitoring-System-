#include <cassert>
#include <iostream>

#include "Event.h"
#include "Config.h"
#include "RuleEngine.h"

int main()
{
    Event failedLogin(
        4625,
        "2026-08-28 15:00:00",
        "Windows Security",
        "Administrator",
        "Failed login attempt",
        HIGH
    );

    assert(failedLogin.getEventId() == 4625);
    assert(failedLogin.getUsername() == "Administrator");
    assert(failedLogin.getSeverity() == HIGH);

    Event successfulLogin(
        4624,
        "2026-08-28 15:05:00",
        "Windows Security",
        "Administrator",
        "Successful login",
        INFO
    );

    assert(successfulLogin.getEventId() == 4624);
    assert(successfulLogin.getSeverity() == INFO);

    Config config;

    assert(config.isValid());
    assert(config.hasRule(4624));
    assert(config.hasRule(4625));
    assert(config.hasRule(4720));
    assert(config.hasRule(4672));

    RuleEngine engine(config);

    assert(engine.isSuspicious(failedLogin));
    assert(!engine.isSuspicious(successfulLogin));

    std::cout << "Event and RuleEngine tests passed."
              << std::endl;

    return 0;
}
