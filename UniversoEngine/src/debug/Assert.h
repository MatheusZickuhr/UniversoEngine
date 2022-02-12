#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace engine {
#ifdef DEBUG

	void assertFileExists(
		const std::string& filePath,
		const std::string& file,
		const long line);

	void assertm(
		const std::string& conditionStr,
		const std::string& message,
		const bool condition,
		const std::string& file,
		const long line);

	void assertFileExtension(
		const std::string& filePath,
		const std::vector<std::string>& validFileExtensions,
		const std::string& file,
		const long line);


#define ASSERT(condition, message) assertm(#condition, message, condition, __FILE__, __LINE__)

#define ASSERT_FILE_EXISTS(filePath) assertFileExists(filePath, __FILE__, __LINE__)

#define ASSERT_FILE_EXTENSION(filePath, ...) assertFileExtension(filePath, __VA_ARGS__, __FILE__, __LINE__)

#else

#define ASSERT(condition, message) do { } while (false)

#define ASSERT_FILE_EXISTS(filePath) do { } while (false)

#define ASSERT_FILE_EXTENSION(filePath, ...) do { } while (false)

#endif
}