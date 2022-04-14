#include "Assert.h"
#include <format>

namespace engine {

#ifdef DEBUG

	void assertFileExists(
		const std::string& filePath,
		const std::string& file,
		const long line) {

		if (!std::filesystem::exists(filePath)) {
			std::cerr << std::format("Attempted to use a non existing file {} in {} line {}",
				filePath, file, line) << std::endl;

			__debugbreak();
			std::terminate();
		}
	}

	void assertm(
		const std::string& conditionStr,
		const std::string& message,
		const bool condition,
		const std::string& file,
		const long line) {

		if (!(condition)) {
			std::cerr << std::format("Assertion `{}` failed with error messege `{}` in {} line {}",
				conditionStr, message, file, line) << std::endl;

			__debugbreak();
			std::terminate();
		}
	}

	void assertFileExtension(
		const std::string& filePath,
		const std::vector<std::string>& validFileExtensions,
		const std::string& file,
		const long line) {

		std::string fileExtension = std::filesystem::path(filePath).extension().string();

		for (auto& validFileExtension : validFileExtensions) {
			if (fileExtension == validFileExtension) {
				return;
			}
		}

		std::string validFileExtensionsStr;
		auto validFileExtensionsSize = validFileExtensions.size();

		for (size_t i = 0; i < validFileExtensionsSize; i++) {
			validFileExtensionsStr += validFileExtensions[i];
			if (i + 1 < validFileExtensionsSize)
				validFileExtensionsStr += " ";
		}

		std::cerr << std::format(
			"Attempted to use file {} in {} line {} with an invalid extension {}, the valid extensions are {}",
			filePath, file, line, fileExtension, validFileExtensionsStr) << std::endl;

		__debugbreak();
		std::terminate();
	}

#endif
}