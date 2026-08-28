#include <cassert>
#include <fstream>
#include <iostream>
#include <cstdio>

#include "LogReader.h"

int main()
{
    const std::string testFile =
        "tests/test_log_reader.log";

    {
        std::ofstream file(testFile);

        file << "2026-08-28 15:00:00|4625|Windows Security|Administrator|Failed login attempt|HIGH\n";

        file << "2026-08-28 15:05:00|4624|Windows Security|Administrator|Successful login|INFO\n";

        file << "\n";

        file << "INVALID LOG ENTRY\n";

        file << "2026-08-28 15:10:00|INVALID|Windows Security|Guest|Invalid event ID|MEDIUM\n";

        file << "2026-08-28 15:15:00|4720|Windows Security|Guest|New user account created|INVALID\n";

        file << "2026-08-28 15:20:00|4720|Windows Security|Guest|New user account created|MEDIUM\n";
    }

    LogReader reader;

    std::vector<Event> events =
        reader.readFile(testFile);

    assert(events.size() == 3);

    assert(events[0].getEventId() == 4625);
    assert(events[0].getUsername() == "Administrator");
    assert(events[0].getSeverity() == HIGH);

    assert(events[1].getEventId() == 4624);
    assert(events[1].getSeverity() == INFO);

    assert(events[2].getEventId() == 4720);
    assert(events[2].getUsername() == "Guest");
    assert(events[2].getSeverity() == MEDIUM);

    std::remove(testFile.c_str());

    std::vector<Event> missingFile =
        reader.readFile("tests/file_that_does_not_exist.log");

    assert(missingFile.empty());

    std::cout
        << "LogReader robustness tests passed."
        << std::endl;

    return 0;
}
