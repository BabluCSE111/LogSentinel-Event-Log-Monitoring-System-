#include <cassert>
#include <iostream>
#include <sstream>

#include "Alert.h"

int main()
{
    Alert alert;

    Event mediumEvent(
        4720,
        "2026-08-28 15:30:00",
        "Windows Security",
        "Guest",
        "New user account created",
        MEDIUM
    );

    Event highEvent(
        4625,
        "2026-08-28 15:31:00",
        "Windows Security",
        "Hp",
        "Failed login attempt",
        HIGH
    );

    Event criticalEvent(
        4672,
        "2026-08-28 15:32:00",
        "Windows Security",
        "Administrator",
        "Critical privilege activity",
        CRITICAL
    );

    std::streambuf* originalBuffer =
        std::cout.rdbuf();

    std::ostringstream output;

    std::cout.rdbuf(
        output.rdbuf()
    );

    alert.showAlert(mediumEvent);
    alert.showAlert(highEvent);
    alert.showAlert(criticalEvent);

    std::cout.rdbuf(
        originalBuffer
    );

    std::string result =
        output.str();

    assert(
        result.find("SECURITY WARNING")
        != std::string::npos
    );

    assert(
        result.find("SECURITY ALERT")
        != std::string::npos
    );

    assert(
        result.find("CRITICAL SECURITY ALERT")
        != std::string::npos
    );

    assert(
        result.find("Severity: MEDIUM")
        != std::string::npos
    );

    assert(
        result.find("Severity: HIGH")
        != std::string::npos
    );

    assert(
        result.find("Severity: CRITICAL")
        != std::string::npos
    );

    std::cout
        << "Alert severity tests passed."
        << std::endl;

    return 0;
}
