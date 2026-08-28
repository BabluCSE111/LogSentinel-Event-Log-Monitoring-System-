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

    Rule rule4624 =
        config.getRule(4624);

    assert(rule4624.severity == INFO);
    assert(!rule4624.suspicious);
    assert(rule4624.message == "Successful login");

    Rule rule4625 =
        config.getRule(4625);

    assert(rule4625.severity == HIGH);
    assert(rule4625.suspicious);
    assert(rule4625.message == "Failed login attempt");

    Rule rule4720 =
        config.getRule(4720);

    assert(rule4720.severity == MEDIUM);
    assert(rule4720.suspicious);
    assert(rule4720.message == "New user account created");

    Rule rule4672 =
        config.getRule(4672);

    assert(rule4672.severity == INFO);
    assert(!rule4672.suspicious);
    assert(rule4672.message == "Special privileges assigned");

    RuleEngine engine(config);

    assert(engine.isSuspicious(failedLogin));
    assert(!engine.isSuspicious(successfulLogin));

    std::cout
        << "Event, Config and RuleEngine tests passed."
        << std::endl;

    return 0;
}
