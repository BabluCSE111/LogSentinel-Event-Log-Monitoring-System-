# LogSentinel – Windows Event Log Monitoring System

LogSentinel is a C++ based Windows Event Log Monitoring System designed to detect suspicious security events, generate alerts, correlate repeated failed login attempts, and monitor the Windows Security Event Log in real time.

The project demonstrates practical cybersecurity concepts including event monitoring, rule-based detection, alert management, event correlation, configuration-driven rules, persistence, and automated testing.

---

## Features

* Real-time Windows Security Event Log monitoring
* Rule-based suspicious event detection
* Configurable event IDs and severity levels
* Security alert generation
* Alert persistence using `logs/alerts.log`
* Duplicate alert prevention
* Failed-login event correlation
* Automated testing using CTest
* Live monitoring through the Windows Event Log API
* Graceful shutdown using `Ctrl+C`

---

## Architecture

LogSentinel is divided into several components:

```text
Windows Security Event Log
            |
            v
      EventMonitor
            |
            v
        Event
            |
            v
      EventProcessor
        /       \
       v         v
 RuleEngine   EventCorrelator
       |         |
       v         v
    AlertManager
       |
       v
 logs/alerts.log
```

### Components

#### Event

Represents a security event containing:

* Event ID
* Timestamp
* Source
* Username
* Message
* Severity

#### Config

Loads and validates monitoring rules from:

```text
config/rules.conf
```

#### RuleEngine

Determines whether an event should be considered suspicious based on configured rules.

#### LogReader

Reads test events from log files and converts them into `Event` objects.

#### EventMonitor

Uses the Windows Event Log API to monitor the Windows Security Event Log in real time.

#### EventProcessor

Coordinates event processing between the rule engine, alert manager, and correlation engine.

#### AlertManager

Responsible for:

* Storing alerts
* Loading persisted alerts
* Preventing duplicate alerts
* Writing alerts to `logs/alerts.log`
* Displaying alert statistics

#### EventCorrelator

Tracks repeated failed login attempts for individual usernames.

A correlation alert is generated when a username reaches **3 failed login attempts**.

---

## Monitored Windows Events

The current configuration monitors these Windows Security Event IDs:

| Event ID | Description                 |
| -------: | --------------------------- |
|     4624 | Successful login            |
|     4625 | Failed login attempt        |
|     4672 | Special privileges assigned |
|     4720 | New user account created    |

The monitored events are defined in:

```text
config/rules.conf
```

---

## Severity Levels

LogSentinel supports five severity levels:

| Severity | Purpose                         |
| -------- | ------------------------------- |
| INFO     | Informational event             |
| LOW      | Low-risk event                  |
| MEDIUM   | Security warning                |
| HIGH     | Suspicious security event       |
| CRITICAL | Highly important security event |

---

## Event Correlation

LogSentinel performs basic event correlation for failed login attempts.

For Event ID `4625`, the system maintains a failed-login counter for each username.

Example:

```text
Failed login #1
Failed login #2
Failed login #3
        |
        v
CORRELATION ALERT
```

Example output:

```text
CORRELATION ALERT: Hp has reached 3 failed login attempts.
```

This demonstrates how individual events can be combined to identify potentially suspicious behavior.

---

## Alert Persistence

Detected alerts are stored in:

```text
logs/alerts.log
```

Example:

```text
Event ID: 4625
Timestamp: 2026-08-28 15:00:00
Username: Administrator
Reason: Failed login attempt
Severity: HIGH
--------------------------------
```

Alerts are loaded when the application starts, allowing previously detected alerts to remain available across program executions.

---

## Requirements

* Windows 10 or later
* C++17 compatible compiler
* CMake 3.15 or later
* Ninja or another supported CMake build system
* Windows Event Log access
* Administrator privileges may be required for Security Event Log monitoring

The project was developed and tested using:

* MSYS2 UCRT64
* GCC
* CMake
* Ninja

---

## Build

From the project directory:

```bash
cmake -S . -B build -G Ninja
```

Build the project:

```bash
cmake --build build
```

The executable will be generated as:

```text
build/LogSentinel.exe
```

---

## Run

Run the application with:

```bash
./build/LogSentinel.exe
```

The application first processes the test log:

```text
logs/test.log
```

It then displays the current alert statistics and starts real-time Windows Security Event Log monitoring.

Example:

```text
========================================
      LOGSENTINEL SECURITY MONITOR
========================================
Starting Windows Event Log monitoring...
Monitoring Windows Security Event Log...
Waiting for security events...
Press Ctrl+C to stop monitoring.
```

Press:

```text
Ctrl+C
```

to stop monitoring gracefully.

---

## Testing

LogSentinel uses CTest for automated testing.

Run all tests:

```bash
ctest --test-dir build --output-on-failure
```

Current test suite:

```text
1. EventAndRuleEngineTest
2. AlertManagerTest
3. LogReaderTest
4. EventProcessorTest
5. AlertSeverityTest
6. EventCorrelatorTest
```

All six tests currently pass successfully:

```text
100% tests passed
```

---

## Project Structure

```text
LogSentinel-Event-Log-Monitoring-System/
│
├── CMakeLists.txt
├── README.md
│
├── config/
│   └── rules.conf
│
├── include/
│   ├── Alert.h
│   ├── AlertManager.h
│   ├── Config.h
│   ├── Event.h
│   ├── EventCorrelator.h
│   ├── EventMonitor.h
│   ├── EventProcessor.h
│   ├── LogReader.h
│   └── RuleEngine.h
│
├── src/
│   ├── Config.cpp
│   ├── Event.cpp
│   ├── EventCorrelator.cpp
│   ├── EventMonitor.cpp
│   ├── EventProcessor.cpp
│   ├── LogReader.cpp
│   └── main.cpp
│
├── tests/
│   ├── test_alert.cpp
│   ├── test_alert_manager.cpp
│   ├── test_event.cpp
│   ├── test_event_correlator.cpp
│   ├── test_event_processor.cpp
│   └── test_log_reader.cpp
│
└── logs/
    ├── alerts.log
    └── test.log
```

---

## Example Detection

When repeated failed login attempts occur:

```text
[EVENT DETECTED]
Event ID   : 4625
Username   : Hp
Severity   : HIGH
Status     : SUSPICIOUS
Message    : Failed login attempt
```

After three failed attempts:

```text
[CORRELATION ALERT]
Username      : Hp
Failed Logins : 3
Status        : THRESHOLD EXCEEDED
```

This allows LogSentinel to identify repeated authentication failures rather than treating each event completely independently.

---

## Security Concepts Demonstrated

This project demonstrates several practical cybersecurity and system-monitoring concepts:

* Windows Event Log monitoring
* Authentication monitoring
* Failed login detection
* Rule-based detection
* Severity classification
* Alert generation
* Event correlation
* Log persistence
* Duplicate detection
* Configuration-driven monitoring
* Real-time event processing
* Automated software testing

---

## Future Improvements

Potential future enhancements include:

* Time-window based event correlation
* IP-based correlation
* Brute-force detection
* Configurable correlation thresholds
* Email or webhook notifications
* JSON/CSV alert export
* Dashboard or graphical interface
* More Windows Security Event IDs
* Improved event parsing
* Alert filtering and searching
* Log rotation
* Multi-threaded event processing

---

## Project Status

Current implementation includes:

* Event model
* Configuration system
* Rule engine
* Log file reader
* Alert system
* Alert persistence
* Event processor
* Event correlation
* Windows Event Log monitoring
* Automated test suite
* Real-time security event detection

**Status: Functional prototype / portfolio cybersecurity project**
