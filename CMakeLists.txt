cmake_minimum_required(VERSION 3.15)

project(LogSentinel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LogSentinel
    src/main.cpp
    src/Event.cpp
    src/LogReader.cpp
    src/EventMonitor.cpp
    src/Config.cpp
    src/EventProcessor.cpp
    src/EventCorrelator.cpp
    src/EventCorrelator.cpp
)

target_include_directories(LogSentinel PRIVATE
    include
)

if(WIN32)
    target_link_libraries(LogSentinel PRIVATE wevtapi)
endif()

enable_testing()

add_executable(test_event
    tests/test_event.cpp
    src/Event.cpp
    src/Config.cpp
)

target_include_directories(test_event PRIVATE
    include
)

add_test(
    NAME EventAndRuleEngineTest
    COMMAND test_event
)

set_tests_properties(
    EventAndRuleEngineTest
    PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(test_alert_manager
    tests/test_alert_manager.cpp
    src/Event.cpp
)

target_include_directories(test_alert_manager PRIVATE
    include
)

add_test(
    NAME AlertManagerTest
    COMMAND test_alert_manager
)

set_tests_properties(
    AlertManagerTest
    PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(test_log_reader
    tests/test_log_reader.cpp
    src/Event.cpp
    src/LogReader.cpp
)

target_include_directories(test_log_reader PRIVATE
    include
)

add_test(
    NAME LogReaderTest
    COMMAND test_log_reader
)

set_tests_properties(
    LogReaderTest
    PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(test_event_processor
    tests/test_event_processor.cpp
    src/Event.cpp
    src/Config.cpp
    src/EventProcessor.cpp
    src/EventCorrelator.cpp
)

target_include_directories(test_event_processor PRIVATE
    include
)

add_test(
    NAME EventProcessorTest
    COMMAND test_event_processor
)

set_tests_properties(
    EventProcessorTest
    PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(test_alert
    tests/test_alert.cpp
    src/Event.cpp
)

target_include_directories(test_alert PRIVATE
    include
)

add_test(
    NAME AlertSeverityTest
    COMMAND test_alert
)

set_tests_properties(
    AlertSeverityTest
    PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(test_event_correlator
    tests/test_event_correlator.cpp
    src/Event.cpp
    src/EventCorrelator.cpp
)

target_include_directories(test_event_correlator PRIVATE
    include
)

add_test(
    NAME EventCorrelatorTest
    COMMAND test_event_correlator
)

set_tests_properties(
    EventCorrelatorTest
    PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)