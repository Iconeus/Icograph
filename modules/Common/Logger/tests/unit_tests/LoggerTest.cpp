
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
#include "Logger/LoggerConfig.hpp"

using namespace medlog;
using namespace std::chrono_literals;

static constexpr std::string_view LOG_FILE_APP_NAME = "TestApp";
static constexpr std::string_view LOG_FILE_DIR = "./test_logs";
static constexpr std::string_view LOG_FILE_NAME = "test.log";
static constexpr std::chrono::milliseconds POLL_INTERVAL{50};
static constexpr std::chrono::milliseconds POLL_TIMEOUT{200};

static const std::string CONFIGURATION_FILE = "test-configuration-log.cfg";

static constexpr std::string_view TRACE_MESSAGE = "This is a trace message";
static constexpr std::string_view DEBUG_MESSAGE = "This is a debug message";
static constexpr std::string_view INFO_MESSAGE = "This is an info message";
static constexpr std::string_view WARN_MESSAGE = "This is a warn message";
static constexpr std::string_view ERROR_MESSAGE = "This is an error message";
static constexpr std::string_view CRITICAL_MESSAGE = "This is a critical message";

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
             std::string_view searchString,
             std::string_view fileName,
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
bool isLogInFile(std::string_view searchString, std::string_view fileName)
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
 * @brief: Create a configuration file with input content
 *
 * @param: Configuration file content
 *
 * @return True if file creation succeeded. False otherwise.
 */
bool createConfigurationFile(std::string_view content)
{
	bool result{true};

	std::ofstream outFile(CONFIGURATION_FILE);
	if (!outFile)
	{
		std::cerr << "Error: Unable to create file " << CONFIGURATION_FILE << std::endl;
		result = false;
	}
	outFile << content;
	outFile.close();
	return result;
}

/**
 * @brief: Delete a configuration file with input content
 *
 * @return True if file deletion succeeded. False otherwise.
 */
bool deleteConfigurationFile()
{
	bool result{true};

	if (!std::filesystem::remove(CONFIGURATION_FILE))
	{
		std::cerr << "Error: Unable to delete file " << CONFIGURATION_FILE << std::endl;
		result = false;
	}

	return result;
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
	cfg.max_file_size_mebibytes = 1;  // 1 MiB
	cfg.max_files = 5;
	cfg.async_queue_size = 1024;
	cfg.thread_count = 1;
	cfg.level = LogLevel::Debug;  // Trace should not be logged
	cfg.flush_every = std::chrono::seconds(1);
	cfg.pattern = "[%Y-%m-%d %H:%M:%S.%e][%^%t%$][%-8l]%v";
	cfg.enable_separate_error_log = true;
	cfg.enable_user_event_log = true;

	Logger logger{cfg};

	// Verify logger initialization
	CHECK_FALSE(detail::shouldLog(LogLevel::Trace));
	CHECK(detail::shouldLog(LogLevel::Debug));
	CHECK(detail::shouldLog(LogLevel::Info));
	CHECK(detail::shouldLog(LogLevel::Warn));
	CHECK(detail::shouldLog(LogLevel::Error));
	CHECK(detail::shouldLog(LogLevel::Critical));
	CHECK(detail::shouldLogUserEvent());
}

// --------------------------------------------------------------------

TEST_CASE("Logger trace level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Trace;
	Logger logger{cfg};

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
}

// --------------------------------------------------------------------

TEST_CASE("Logger debug level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Debug;
	Logger logger{cfg};

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
}

// --------------------------------------------------------------------

TEST_CASE("Logger info level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Info;
	Logger logger{cfg};

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
}

// --------------------------------------------------------------------

TEST_CASE("Logger warn level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Warn;
	Logger logger{cfg};

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
}

// --------------------------------------------------------------------

TEST_CASE("Logger error level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Error;
	Logger logger{cfg};

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
}

// --------------------------------------------------------------------

TEST_CASE("Logger critical level")
{
	auto cfg = getConfigForTest();

	cfg.level = LogLevel::Critical;
	Logger logger{cfg};

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
}

// --------------------------------------------------------------------

TEST_CASE("Logger user event")
{
	auto cfg = getConfigForTest();

	const std::string userEventLogFileName{"testUserEvent.log"};
	cfg.enable_user_event_log = true;
	cfg.user_event_log_filename = userEventLogFileName;
	Logger logger{cfg};

	// Log a user event
	MEDLOG_USER_EVENT("This is a user event");

	// Verify user event is logged
	CHECK(detail::shouldLogUserEvent());
	CHECK(isLogInFile("This is a user event", userEventLogFileName));
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
	medlog::Logger logger{cfg};

	REQUIRE_THROWS_MATCHES(
	    medlog::Logger(cfg), std::logic_error,
	    Catch::Matchers::Message("Logger already initialized.")  // Expected exit code
	);
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
	    medlog::Logger(cfg),
	    Catch::Matchers::ContainsSubstring("Failed opening file") &&
	        Catch::Matchers::ContainsSubstring("for writing: Permission denied"));

	// Cleanup: Restore permissions and remove the directory
	std::filesystem::permissions(tempDir, std::filesystem::perms::owner_all |
	                                          std::filesystem::perms::group_all |
	                                          std::filesystem::perms::others_all);
	std::filesystem::remove_all(tempDir);
}

// --------------------------------------------------------------------

TEST_CASE("Multithread stress test")
{
	// This test sends numerous messages through 3 threads. Then we check that all
	// messages have been successfully written without overlap between them.

	auto cfg = getConfigForTest();
	medlog::Logger logger{cfg};

	std::vector<std::string> messagesThread1;
	std::vector<std::string> messagesThread2;
	std::vector<std::string> messagesThread3;
	constexpr uint8_t numberOfMessages{50};

	auto triggerLog(
	    [](uint8_t threadId, uint8_t numberMessages, std::vector<std::string>& messages)
	    {
		    for (uint8_t i = 0; i < numberMessages; i++)
		    {
			    std::string myMessage = "My Message number " + std::to_string(i) +
			                            " from thread " + std::to_string(threadId);
			    MEDLOG_INFO(myMessage);
			    messages.push_back(myMessage);
		    }
	    });

	std::thread thread1(triggerLog, 1, numberOfMessages, std::ref(messagesThread1));
	std::thread thread2(triggerLog, 2, numberOfMessages, std::ref(messagesThread2));
	std::thread thread3(triggerLog, 3, numberOfMessages, std::ref(messagesThread3));

	thread1.join();
	thread2.join();
	thread3.join();

	for (uint8_t i = 0; i < numberOfMessages; i++)
	{
		CHECK(isLogInFile(messagesThread1[i], LOG_FILE_NAME));
		CHECK(isLogInFile(messagesThread2[i], LOG_FILE_NAME));
		CHECK(isLogInFile(messagesThread3[i], LOG_FILE_NAME));
	}
}

// --------------------------------------------------------------------

TEST_CASE("Load configuration file")
{
	// clang-format off
	std::string_view content =
	    "app_name = appTest\n" 
		"log_dir = logsTest\n"
		"log_filename = filenameTest\n"
		"error_log_filename = errorFilenameTest\n"
		"user_event_name = userEventNameTest\n"
		"user_event_log_filename = userEventFilenameTest\n"
		"max_file_size_mebibytes = 10\n"
		"max_files = 2\n"
		"async_queue_size = 5000\n"
		"thread_count = 3\n"
		"level = Debug\n"
		"flush_every = 100\n"
		"enable_separate_error_log = false\n"
		"enable_user_event_log = true\n"

;
	// clang-format on
	CHECK(createConfigurationFile(content));

	std::filesystem::path configurationFilePath(CONFIGURATION_FILE);

	auto result = medlog::loadConfigurationFile(configurationFilePath);
	CHECK(result);

	LoggerConfig config = result.value();
	CHECK(config.app_name == "appTest");
	CHECK(config.log_dir == "logsTest");
	CHECK(config.log_filename == "filenameTest");
	CHECK(config.error_log_filename == "errorFilenameTest");
	CHECK(config.user_event_name == "userEventNameTest");
	CHECK(config.user_event_log_filename == "userEventFilenameTest");
	CHECK(config.max_file_size_mebibytes == 10);
	CHECK(config.max_files == 2);
	CHECK(config.async_queue_size == 5000);
	CHECK(config.thread_count == 3);
	CHECK(config.level == LogLevel::Debug);
	CHECK(config.flush_every == std::chrono::milliseconds(100));
	CHECK(config.enable_separate_error_log == false);
	CHECK(config.enable_user_event_log == true);

	CHECK(deleteConfigurationFile());
}

// --------------------------------------------------------------------

TEST_CASE("Load configuration file with wrong boolean input")
{
	// clang-format off
	std::string_view content = "enable_separate_error_log = test\n";
	// clang-format on
	CHECK(createConfigurationFile(content));

	std::filesystem::path configurationFilePath(CONFIGURATION_FILE);

	auto result = medlog::loadConfigurationFile(configurationFilePath);
	CHECK_FALSE(result);
	CHECK(result.error() ==
	      "loadConfigurationFile: Invalid value test for key enable_separate_error_log");

	CHECK(deleteConfigurationFile());
}

// --------------------------------------------------------------------

TEST_CASE("Load configuration file with wrong LogLevel input")
{
	// clang-format off
	std::string_view content = "level = test\n";
	// clang-format on
	CHECK(createConfigurationFile(content));

	std::filesystem::path configurationFilePath(CONFIGURATION_FILE);

	auto result = medlog::loadConfigurationFile(configurationFilePath);
	CHECK_FALSE(result);
	CHECK(result.error() == "loadConfigurationFile: Invalid value test for key level");

	CHECK(deleteConfigurationFile());
}

// --------------------------------------------------------------------

TEST_CASE("Load configuration file with wrong size_t input")
{
	// clang-format off
	std::string_view content = "thread_count = test\n";
	// clang-format on
	CHECK(createConfigurationFile(content));

	std::filesystem::path configurationFilePath(CONFIGURATION_FILE);

	auto result = medlog::loadConfigurationFile(configurationFilePath);
	CHECK_FALSE(result);
	CHECK(result.error() ==
	      "loadConfigurationFile: Invalid value test for key thread_count");

	CHECK(deleteConfigurationFile());
}

// --------------------------------------------------------------------

TEST_CASE("Load configuration file with wrong key")
{
	// clang-format off
	std::string_view content = "unknown_key = test\n";
	// clang-format on
	CHECK(createConfigurationFile(content));

	std::filesystem::path configurationFilePath(CONFIGURATION_FILE);

	auto result = medlog::loadConfigurationFile(configurationFilePath);
	CHECK_FALSE(result);
	CHECK(result.error() == "loadConfigurationFile: Invalid key unknown_key");

	CHECK(deleteConfigurationFile());
}
