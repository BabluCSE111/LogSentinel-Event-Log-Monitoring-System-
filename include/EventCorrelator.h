#ifndef EVENTCORRELATOR_H
#define EVENTCORRELATOR_H

#include <map>
#include <string>

#include "Event.h"

class EventCorrelator
{
private:
    std::map<std::string, int> failedLoginCounts;

public:
    bool recordEvent(const Event& event);

    int getFailedLoginCount(
        const std::string& username
    ) const;

    void reset(
        const std::string& username
    );
};

#endif
