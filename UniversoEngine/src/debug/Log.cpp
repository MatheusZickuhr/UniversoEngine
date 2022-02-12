#include "Log.h"
#include <iostream>
#include <filesystem>
#include <ctime>
#include <format>

namespace engine {

#ifdef DEBUG

	void log(const std::string& message, const std::string& filePath, const long line) {
		std::string fileName = std::filesystem::path(filePath).filename().string();
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);

		std::cout << std::format(
			"{}-{}-{} {}:{}:{} {} {}: {}",
			(now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday,
			now->tm_hour, now->tm_min, now->tm_sec,
			fileName, line, message) << std::endl;
	}


#endif

}

