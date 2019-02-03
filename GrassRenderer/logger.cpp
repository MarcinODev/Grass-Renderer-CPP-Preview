#include <cstdio>
#include <fstream>
#include <memory>
#include <functional>
#include <chrono>
#include <ctime>   
#include "logger.h"

Logger* Logger::instance;
const char* logPath = "logs.log";

Logger::Logger()
{
	instance = this;
	std::ostream* stream = new std::ofstream(logPath, std::ofstream::out | std::ofstream::trunc);
	auto deleter = [](std::ostream* stream){delete stream;};
	outputStream = std::unique_ptr< std::ostream, std::function<void(std::ostream*)> >(stream, deleter);

    auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	(*outputStream) << "Started at: " << std::ctime(&time) << std::endl;
	
}

Logger::~Logger()
{
}

void Logger::log(const std::string& text, const std::string& funcName, const std::string& fileName, unsigned int line, LogType logType)
{
	std::string log = text + " @@" + fileName + ":" + funcName + "():" + std::to_string(line);
	switch(logType)
	{
		case LogType::Debug:
			debug(log);
			break;
		case LogType::Warning:
			warning(log);
			break;
		case LogType::error:
			error(log);
			break;
		default:
			break;
	}
}

void Logger::debug(const std::string& text)
{
	checkInstance();
	(*instance->outputStream) << "[DEBUG]" << text << std::endl;
}

void Logger::warning(const std::string& text)
{
	checkInstance();
	(*instance->outputStream) << "[WARN]" << text << std::endl;
}

void Logger::error(const std::string& text)
{
	checkInstance();
	(*instance->outputStream) << "[ERROR]" << text << std::endl;
}

std::string Logger::hresultToString(HRESULT result)
{
	_com_error err(result);
	LPCTSTR msg = err.ErrorMessage();
	std::wstring wstr = std::wstring(msg);
	return std::string(wstr.begin(), wstr.end());
}

void Logger::checkInstance()
{
	if(instance == nullptr)
	{
		instance = new Logger();
	}
}
