#include "Assert.h"

#ifdef _DEBUG

void assertFileExists(
	const std::string& filePath,
	const std::string& file,
	const long line) {

	if (!std::filesystem::exists(filePath)) {
		std::cerr << "Attempted to use a non existing file `" << filePath
			<< "` in " << file << ", line " << line << std::endl;
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
		std::cerr << "Assertion `" << conditionStr << "` failed with error messege `" << message << "` in " << file
			<< ", line " << line << std::endl;
		std::terminate();
	}
}

void assertFileExtension(
	const std::string& filePath,
	const std::vector<std::string>& validFileExtensions,
	const std::string& file,
	const long line) {

	std::string fileExtension = std::filesystem::path(filePath).extension().string();

	for (auto validFileExtension : validFileExtensions) {
		if (fileExtension == validFileExtension) {
			return;
		}
	}

	std::string validFileExtensionsStr;
	auto validFileExtensionsSize = validFileExtensions.size();

	for (int i = 0; i < validFileExtensionsSize; i++) {
		validFileExtensionsStr += validFileExtensions[i];
		if (i + 1 < validFileExtensionsSize)
			validFileExtensionsStr += " ";
	}

	std::cerr << "Attempted to use file `" << filePath <<
		"` in " << file << ", line " << line <<
		" with an invalid extension `" << fileExtension <<
		"`, the valid extensions are `" << validFileExtensionsStr << "`" << std::endl;
	std::terminate();

}

#endif