#ifndef LOGREADER_H
#define LOGREADER_H

#include <string>
#include <vector>

#include "Event.h"

class LogReader
{
public:
    std::vector<Event> readFile(
        const std::string& filePath
    );
};

#endif
