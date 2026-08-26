#ifndef RULEENGINE_H
#define RULEENGINE_H

#include "Event.h"

class RuleEngine {
public:

    bool isSuspicious(Event event) {

        if (event.getSeverity() == HIGH ||
            event.getSeverity() == CRITICAL ||
            event.getEventId() == 4625) {

            return true;
        }

        return false;
    }
};

#endif