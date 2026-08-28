#include <iostream>
#include <vector>

#include "LogReader.h"
#include "RuleEngine.h"
#include "AlertManager.h"
#include "EventMonitor.h"
#include "Config.h"
#include "EventProcessor.h"
#include "EventCorrelator.h"

using namespace std;

int main()
{
    Config config;

    if (!config.isValid())
    {
        cout << "ERROR: Configuration is invalid."
             << endl;

        return 1;
    }

    AlertManager alertManager;

    alertManager.loadAlerts();

    RuleEngine engine(config);

    EventCorrelator correlator;

    EventProcessor processor(
        engine,
        alertManager,
        correlator
    );

    LogReader reader;

    vector<Event> events =
        reader.readFile("logs/test.log");

    for (const Event& event : events)
    {
        processor.process(event);
    }

    alertManager.showStatistics();

    EventMonitor monitor(
        [&](Event event)
        {
            cout << "\nReal event received: "
                 << event.getEventId()
                 << endl;

            processor.process(event);
        },
        config
    );

    monitor.start();

    return 0;
}
