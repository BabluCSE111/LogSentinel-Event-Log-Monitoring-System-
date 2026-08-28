#include "EventCorrelator.h"

bool EventCorrelator::recordEvent(
    const Event& event
)
{
    if (event.getEventId() != 4625)
    {
        return false;
    }

    const std::string username =
        event.getUsername();

    if (username.empty())
    {
        return false;
    }

    failedLoginCounts[username]++;

    return failedLoginCounts[username] == 3;
}

int EventCorrelator::getFailedLoginCount(
    const std::string& username
) const
{
    auto it =
        failedLoginCounts.find(username);

    if (it == failedLoginCounts.end())
    {
        return 0;
    }

    return it->second;
}

void EventCorrelator::reset(
    const std::string& username
)
{
    failedLoginCounts.erase(username);
}
