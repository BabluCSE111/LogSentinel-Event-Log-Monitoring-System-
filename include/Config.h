#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <vector>

#include "Event.h"

struct Rule
{
    Severity severity;
    bool suspicious;
    std::string message;
};

class Config
{
private:
    std::map<int, Rule> eventRules;
    bool valid;

    bool isValidSeverity(const std::string& value) const;

public:
    Config();

    explicit Config(const std::string& filePath);

    Rule getRule(int eventId) const;

    bool hasRule(int eventId) const;

    std::vector<int> getEventIds() const;

    bool isValid() const;
};

#endif
