
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <catch2/catch_all.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include "Logger/Logger.hpp"

using namespace medlog;

static constexpr std::string LOG_FILE_DIR = "./test_logs";
static constexpr std::string LOG_FILE_NAME = "test.log";

LoggerConfig getConfigForTest()
{
	LoggerConfig cfg;
	cfg.app_name = "TestApp";
	cfg.log_dir = LOG_FILE_DIR;
	cfg.log_filename = LOG_FILE_NAME;

	return cfg;
}

bool isLogInFile(const std::string& searchString, const std::filesystem::path& filePath)
{
	bool result{false};
	std::cout << "Check file: " << filePath.c_str() << std::endl;

	std::ifstream file(filePath);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			if (line.find(searchString) != std::string::npos)
			{
				result = true;
				break;
			}
		}
	}
	else
	{
		std::cerr << "Error: Could not open file: " << filePath << std::endl;
	}

	file.close();
	return result;
}

void cleanLogFileDirectory()
{
	if (std::filesystem::exists(LOG_FILE_DIR))
	{
		std::filesystem::remove_all(LOG_FILE_DIR);
	}
}

class testRunListener : public Catch::EventListenerBase
{
public:
	using Catch::EventListenerBase::EventListenerBase;

	void testCaseEnded(Catch::TestCaseStats const& /*testCaseStats */)
	{
		// cleanLogFileDirectory();
	}
};

CATCH_REGISTER_LISTENER(testRunListener)

TEST_CASE("Logger initialization")
{
	auto cfg = getConfigForTest();
	cfg.error_log_filename = "test_error.log";
	cfg.user_event_name = "TestUserEvent";
	cfg.user_event_log_filename = "test_user_event.log";
	cfg.max_file_size_bytes = 1024 * 1024;  // 1 MB
	cfg.max_files = 5;
	cfg.async_queue_size = 1024;
	cfg.thread_count = 1;
	cfg.level = LogLevel::Trace;
	cfg.flush_every = std::chrono::seconds(1);
	cfg.pattern = "[%Y-%m-%d %H:%M:%S.%e][%^%t%$][%-8l]%v";
	cfg.enable_separate_error_log = true;
	cfg.enable_user_event_log = true;

	initLogger(cfg);

	// Verify logger initialization
	CHECK(detail::shouldLog(LogLevel::Trace));
	CHECK(detail::shouldLogUserEvent());

	shutdown();
}

TEST_CASE("Logger trace level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Trace;
	initLogger(cfg);

	MEDLOG_TRACE("This is a trace message");

	CHECK(detail::shouldLog(LogLevel::Trace));
	CHECK(isLogInFile("This is a trace message", LOG_FILE_DIR + "/" + LOG_FILE_NAME));

	shutdown();
}

// TEST_CASE("Logger debug level")
// {
// 	auto cfg = getConfigForTest();

// 	cfg.level = LogLevel::Debug;
// 	initLogger(cfg);

// 	// Log a debug message
// 	MEDLOG_DEBUG("This is a debug message");

// 	// Verify debug message is logged
// 	CHECK(detail::shouldLog(LogLevel::Debug));

// 	shutdown();
// }

// TEST_CASE("Logger info level")
// {
// 	auto cfg = getConfigForTest();

// 	cfg.level = LogLevel::Info;
// 	initLogger(cfg);

// 	// Log an info message
// 	MEDLOG_INFO("This is an info message");

// 	// Verify info message is logged
// 	CHECK(detail::shouldLog(LogLevel::Info));

// 	shutdown();
// }

// TEST_CASE("Logger warn level")
// {
// 	auto cfg = getConfigForTest();

// 	cfg.level = LogLevel::Warn;
// 	initLogger(cfg);

// 	// Log a warn message
// 	MEDLOG_WARN("This is a warn message");

// 	// Verify warn message is logged
// 	CHECK(detail::shouldLog(LogLevel::Warn));

// 	shutdown();
// }

// TEST_CASE("Logger error level")
// {
// 	auto cfg = getConfigForTest();

// 	cfg.level = LogLevel::Error;
// 	initLogger(cfg);

// 	// Log an error message
// 	MEDLOG_ERROR("This is an error message");

// 	// Verify error message is logged
// 	CHECK(detail::shouldLog(LogLevel::Error));

// 	shutdown();
// }

// TEST_CASE("Logger critical level")
// {
// 	auto cfg = getConfigForTest();

// 	cfg.level = LogLevel::Critical;
// 	initLogger(cfg);

// 	// Log a critical message
// 	MEDLOG_CRITICAL("This is a critical message");

// 	// Verify critical message is logged
// 	CHECK(detail::shouldLog(LogLevel::Critical));

// 	shutdown();
// }

// TEST_CASE("Logger user event")
// {
// 	auto cfg = getConfigForTest();

// 	cfg.enable_user_event_log = true;
// 	initLogger(cfg);

// 	// Log a user event
// 	MEDLOG_USER_EVENT("This is a user event");

// 	// Verify user event is logged
// 	CHECK(detail::shouldLogUserEvent());

// 	shutdown();
// }
