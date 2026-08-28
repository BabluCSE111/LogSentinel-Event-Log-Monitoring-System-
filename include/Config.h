#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>

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
    bool valid;

    bool isValidSeverity(const std::string& value) const;

public:
    Config();

    Rule getRule(int eventId) const;

    bool hasRule(int eventId) const;

    bool isValid() const;
};

#endif
