#pragma once

#include <string>

namespace engine {

#ifdef DEBUG

	void log(
		const std::string& message,
		const std::string& filePath,
		const long line);

#define LOG(message) log(message, __FILE__, __LINE__)

#else

#define LOG(message) do { } while (false)

#endif

}