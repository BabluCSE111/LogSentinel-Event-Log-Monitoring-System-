#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include "Event.h"

struct Rule
{
    Severity severity;
    bool suspicious;
};

class Config
{
private:
    std::map<int, Rule> eventRules;

public:
    Config();

    Rule getRule(int eventId) const;

    bool hasRule(int eventId) const;
};

#endif