
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <stdexcept>
#include <string>

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include "Logger/Logger.hpp"

using namespace medlog;
using namespace std::chrono_literals;

static const std::string LOG_FILE_APP_NAME = "TestApp";
static const std::string LOG_FILE_DIR = "./test_logs";
static const std::string LOG_FILE_NAME = "test.log";
static constexpr std::chrono::milliseconds POLL_INTERVAL{100};
static constexpr std::chrono::milliseconds POLL_TIMEOUT{500};

static const std::string TRACE_MESSAGE = "This is a trace message";
static const std::string DEBUG_MESSAGE = "This is a debug message";
static const std::string INFO_MESSAGE = "This is an info message";
static const std::string WARN_MESSAGE = "This is a warn message";
static const std::string ERROR_MESSAGE = "This is an error message";
static const std::string CRITICAL_MESSAGE = "This is a critical message";

/**
 * @brief: Generate the basic Logger configuration for unit tests to ensure that logfiles
 * are generated in a safe place.
 */
LoggerConfig getConfigForTest()
{
	LoggerConfig cfg;
	cfg.app_name = LOG_FILE_APP_NAME;
	cfg.log_dir = LOG_FILE_DIR;
	cfg.log_filename = LOG_FILE_NAME;
	cfg.flush_every = 50ms;

	return cfg;
}

/**
 * @brief: This function will poll the logfile until the input pattern is found. If
 * found, the std::promise value will be set. Otherwise the promise is never fulfilled.
 *
 * @param promise the promise to be fullfiled. To be set to true as soon as log is found.
 * @param searchString pattern to be found in the log
 * @param fileName logfile to parse
 * @param stopFlag flag for the timeout
 */
void pollLog(std::promise<bool>&& promise,
             const std::string& searchString,
             const std::string& fileName,
             std::atomic<bool>& stopFlag)
{
	std::filesystem::path filePath{LOG_FILE_DIR};
	filePath /= fileName;

	// Search for the log until found or timeout has expired
	bool found{false};
	while (!found && !stopFlag)
	{
		std::ifstream file(filePath);
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				if (line.find(searchString) != std::string::npos)
				{
					found = true;
					break;
				}
			}
			file.close();
		}
		else
		{
			std::cerr << "Error: Could not open file: " << filePath << std::endl;
		}

		// If log found, set the promise value.
		// Sleep and retry otherwise
		if (found)
		{
			promise.set_value(true);
		}
		else
		{
			std::this_thread::sleep_for(POLL_INTERVAL);
		}
	}
}

/**
 * @brief: Check if logfile contains the input string pattern. The std::future is used to
 * adapt to the asynchronous logger. As soon as the log is found, the promise's value is
 * set and retrieve via its std::future. The future waits for a maximum timeout for the
 * promise to be set before pursuing with the main thread.
 *
 * @param promise the promise to be fullfiled. To be set to true as soon as log is found.
 * @param searchString pattern to be found in the log
 * @param fileName logfile to parse
 * @param stopFlag flag for the timeout
 */
bool isLogInFile(const std::string& searchString, const std::string& fileName)
{
	bool isLogInFile{false};

	auto promise = std::promise<bool>();
	auto future = promise.get_future();
	std::atomic<bool> stopFlag{false};

	std::thread pollThread(pollLog, std::move(promise), searchString, fileName,
	                       std::ref(stopFlag));

	// Wait for the promise to be fulfilled within the timeout
	auto status = future.wait_for(POLL_TIMEOUT);

	if (status == std::future_status::ready)
	{
		isLogInFile = future.get();
	}
	else
	{
		// Force the polling to stop if timeout has expired
		stopFlag = true;
	}

	pollThread.join();
	return isLogInFile;
}

/**
 * @brief: Remove log directory for cleanup
 */
void cleanLogFileDirectory()
{
	if (std::filesystem::exists(LOG_FILE_DIR))
	{
		std::filesystem::remove_all(LOG_FILE_DIR);
	}
}

/**
 * @class: Class used to define events listeners for the unit tests. Useful for setup and
 * teardown functions that shall be executed before or after each test.
 */
class testRunListener : public Catch::EventListenerBase
{
public:
	using Catch::EventListenerBase::EventListenerBase;

	/**
	 * @brief: Function that is called after each test case execution. Used to cleanup
	 * the logfiles created by the tests.
	 */
	void testCaseEnded(Catch::TestCaseStats const& /*testCaseStats */)
	{
		cleanLogFileDirectory();
	}
};

CATCH_REGISTER_LISTENER(testRunListener)

// --------------------------------------------------------------------
// START TESTS
// --------------------------------------------------------------------

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
	cfg.level = LogLevel::Debug;  // Trace should not be logged
	cfg.flush_every = std::chrono::seconds(1);
	cfg.pattern = "[%Y-%m-%d %H:%M:%S.%e][%^%t%$][%-8l]%v";
	cfg.enable_separate_error_log = true;
	cfg.enable_user_event_log = true;

	initLogger(cfg);

	// Verify logger initialization
	CHECK_FALSE(detail::shouldLog(LogLevel::Trace));
	CHECK(detail::shouldLog(LogLevel::Debug));
	CHECK(detail::shouldLog(LogLevel::Info));
	CHECK(detail::shouldLog(LogLevel::Warn));
	CHECK(detail::shouldLog(LogLevel::Error));
	CHECK(detail::shouldLog(LogLevel::Critical));
	CHECK(detail::shouldLogUserEvent());

	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Logger trace level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Trace;
	initLogger(cfg);

	MEDLOG_TRACE(TRACE_MESSAGE);
	MEDLOG_DEBUG(DEBUG_MESSAGE);
	MEDLOG_INFO(INFO_MESSAGE);
	MEDLOG_WARN(WARN_MESSAGE);
	MEDLOG_ERROR(ERROR_MESSAGE);
	MEDLOG_CRITICAL(CRITICAL_MESSAGE);

	CHECK(detail::shouldLog(LogLevel::Trace));
	CHECK(detail::shouldLog(LogLevel::Debug));
	CHECK(detail::shouldLog(LogLevel::Info));
	CHECK(detail::shouldLog(LogLevel::Warn));
	CHECK(detail::shouldLog(LogLevel::Error));
	CHECK(detail::shouldLog(LogLevel::Critical));

	// Verify message is logged when it should
	CHECK(isLogInFile(TRACE_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(DEBUG_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(INFO_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(WARN_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(ERROR_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(CRITICAL_MESSAGE, LOG_FILE_NAME));

	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Logger debug level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Debug;
	initLogger(cfg);

	MEDLOG_TRACE(TRACE_MESSAGE);
	MEDLOG_DEBUG(DEBUG_MESSAGE);
	MEDLOG_INFO(INFO_MESSAGE);
	MEDLOG_WARN(WARN_MESSAGE);
	MEDLOG_ERROR(ERROR_MESSAGE);
	MEDLOG_CRITICAL(CRITICAL_MESSAGE);

	CHECK_FALSE(detail::shouldLog(LogLevel::Trace));
	CHECK(detail::shouldLog(LogLevel::Debug));
	CHECK(detail::shouldLog(LogLevel::Info));
	CHECK(detail::shouldLog(LogLevel::Warn));
	CHECK(detail::shouldLog(LogLevel::Error));
	CHECK(detail::shouldLog(LogLevel::Critical));

	// Verify message is logged when it should
	CHECK_FALSE(isLogInFile(TRACE_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(DEBUG_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(INFO_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(WARN_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(ERROR_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(CRITICAL_MESSAGE, LOG_FILE_NAME));

	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Logger info level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Info;
	initLogger(cfg);

	MEDLOG_TRACE(TRACE_MESSAGE);
	MEDLOG_DEBUG(DEBUG_MESSAGE);
	MEDLOG_INFO(INFO_MESSAGE);
	MEDLOG_WARN(WARN_MESSAGE);
	MEDLOG_ERROR(ERROR_MESSAGE);
	MEDLOG_CRITICAL(CRITICAL_MESSAGE);

	CHECK_FALSE(detail::shouldLog(LogLevel::Trace));
	CHECK_FALSE(detail::shouldLog(LogLevel::Debug));
	CHECK(detail::shouldLog(LogLevel::Info));
	CHECK(detail::shouldLog(LogLevel::Warn));
	CHECK(detail::shouldLog(LogLevel::Error));
	CHECK(detail::shouldLog(LogLevel::Critical));

	// Verify message is logged when it should
	CHECK_FALSE(isLogInFile(TRACE_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(DEBUG_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(INFO_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(WARN_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(ERROR_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(CRITICAL_MESSAGE, LOG_FILE_NAME));

	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Logger warn level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Warn;
	initLogger(cfg);

	MEDLOG_TRACE(TRACE_MESSAGE);
	MEDLOG_DEBUG(DEBUG_MESSAGE);
	MEDLOG_INFO(INFO_MESSAGE);
	MEDLOG_WARN(WARN_MESSAGE);
	MEDLOG_ERROR(ERROR_MESSAGE);
	MEDLOG_CRITICAL(CRITICAL_MESSAGE);

	CHECK_FALSE(detail::shouldLog(LogLevel::Trace));
	CHECK_FALSE(detail::shouldLog(LogLevel::Debug));
	CHECK_FALSE(detail::shouldLog(LogLevel::Info));
	CHECK(detail::shouldLog(LogLevel::Warn));
	CHECK(detail::shouldLog(LogLevel::Error));
	CHECK(detail::shouldLog(LogLevel::Critical));

	// Verify message is logged when it should
	CHECK_FALSE(isLogInFile(TRACE_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(DEBUG_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(INFO_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(WARN_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(ERROR_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(CRITICAL_MESSAGE, LOG_FILE_NAME));

	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Logger error level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Error;
	initLogger(cfg);

	MEDLOG_TRACE(TRACE_MESSAGE);
	MEDLOG_DEBUG(DEBUG_MESSAGE);
	MEDLOG_INFO(INFO_MESSAGE);
	MEDLOG_WARN(WARN_MESSAGE);
	MEDLOG_ERROR(ERROR_MESSAGE);
	MEDLOG_CRITICAL(CRITICAL_MESSAGE);

	CHECK_FALSE(detail::shouldLog(LogLevel::Trace));
	CHECK_FALSE(detail::shouldLog(LogLevel::Debug));
	CHECK_FALSE(detail::shouldLog(LogLevel::Info));
	CHECK_FALSE(detail::shouldLog(LogLevel::Warn));
	CHECK(detail::shouldLog(LogLevel::Error));
	CHECK(detail::shouldLog(LogLevel::Critical));

	// Verify message is logged when it should
	CHECK_FALSE(isLogInFile(TRACE_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(DEBUG_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(INFO_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(WARN_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(ERROR_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(CRITICAL_MESSAGE, LOG_FILE_NAME));
	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Logger critical level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Critical;
	initLogger(cfg);

	MEDLOG_TRACE(TRACE_MESSAGE);
	MEDLOG_DEBUG(DEBUG_MESSAGE);
	MEDLOG_INFO(INFO_MESSAGE);
	MEDLOG_WARN(WARN_MESSAGE);
	MEDLOG_ERROR(ERROR_MESSAGE);
	MEDLOG_CRITICAL(CRITICAL_MESSAGE);

	CHECK_FALSE(detail::shouldLog(LogLevel::Trace));
	CHECK_FALSE(detail::shouldLog(LogLevel::Debug));
	CHECK_FALSE(detail::shouldLog(LogLevel::Info));
	CHECK_FALSE(detail::shouldLog(LogLevel::Warn));
	CHECK_FALSE(detail::shouldLog(LogLevel::Error));
	CHECK(detail::shouldLog(LogLevel::Critical));

	// Verify message is logged when it should
	CHECK_FALSE(isLogInFile(TRACE_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(DEBUG_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(INFO_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(WARN_MESSAGE, LOG_FILE_NAME));
	CHECK_FALSE(isLogInFile(ERROR_MESSAGE, LOG_FILE_NAME));
	CHECK(isLogInFile(CRITICAL_MESSAGE, LOG_FILE_NAME));
	shutdown();

	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Logger user event")
{
	auto cfg = getConfigForTest();

	const std::string userEventLogFileName{"testUserEvent.log"};
	cfg.enable_user_event_log = true;
	cfg.user_event_log_filename = userEventLogFileName;
	initLogger(cfg);

	// Log a user event
	MEDLOG_USER_EVENT("This is a user event");

	// Verify user event is logged
	CHECK(detail::shouldLogUserEvent());
	CHECK(isLogInFile("This is a user event", userEventLogFileName));

	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Call without init")
{
	REQUIRE_THROWS_MATCHES(
	    MEDLOG_USER_EVENT("This is a user event"), std::logic_error,
	    Catch::Matchers::Message(
	        "Logger not initialized. Call initLogger() first.")  // Expected exit code
	);
}

// --------------------------------------------------------------------

TEST_CASE("Call twice the init")
{
	auto cfg = getConfigForTest();
	medlog::initLogger(cfg);

	REQUIRE_THROWS_MATCHES(
	    medlog::initLogger(cfg), std::logic_error,
	    Catch::Matchers::Message("Logger already initialized.")  // Expected exit code
	);
	shutdown();
}

// --------------------------------------------------------------------

TEST_CASE("Try to create log folder in a folder without permissions.")
{
	std::filesystem::path tempDir =
	    std::filesystem::temp_directory_path() / "folder_without_permission";

	// Clean if previous folder exists
	if (std::filesystem::exists(tempDir.parent_path()))
	{
		std::filesystem::remove_all(tempDir);
	}

	// Create the parent directory
	REQUIRE(std::filesystem::create_directory(tempDir));

	// Remove write permissions for everyone
	std::filesystem::permissions(tempDir, std::filesystem::perms::owner_read |
	                                          std::filesystem::perms::group_read |
	                                          std::filesystem::perms::others_read);

	auto cfg = getConfigForTest();
	cfg.log_dir = tempDir;

	// Attempt to create the logfiles (should fail)
	REQUIRE_THROWS_WITH(
	    medlog::initLogger(cfg),
	    Catch::Matchers::ContainsSubstring("Failed opening file") &&
	        Catch::Matchers::ContainsSubstring("for writing: Permission denied"));

	// Cleanup: Restore permissions and remove the directory
	std::filesystem::permissions(tempDir, std::filesystem::perms::owner_all |
	                                          std::filesystem::perms::group_all |
	                                          std::filesystem::perms::others_all);
	std::filesystem::remove_all(tempDir);
}
