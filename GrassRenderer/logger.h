#pragma once
#include <string>
#include <ostream>
#include <memory>
#include <functional>
#include <comdef.h>

#define LogDebug(text) Logger::log(text, __func__, __FILE__, __LINE__, LogType::Debug);
#define LogWarning(text) Logger::log(text, __func__, __FILE__, __LINE__, LogType::Warning);
#define LogError(text) Logger::log(text, __func__, __FILE__, __LINE__, LogType::error);

enum class LogType {Debug, Warning, error};

/// <summary>
/// Singleton for logging events.
/// </summary>
class Logger
{
public:
	static void log(const std::string& text, const std::string& funcName, const std::string& fileName, unsigned int line, LogType logType);
	static void debug(const std::string& text);
	static void warning(const std::string& text);
	static void error(const std::string& text);
	static std::string hresultToString(HRESULT result);

private:
	static Logger* instance;
	
	std::unique_ptr< std::ostream, std::function<void(std::ostream*)> > outputStream;

	Logger();
	~Logger();

	static void checkInstance();
};


