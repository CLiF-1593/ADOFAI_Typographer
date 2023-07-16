#pragma once
#include <string>
#include <vector>
#include <codecvt>
//#include <json/json.h>
#include "Config.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


class FileIO {
public:
	enum FileType {
		TYPO,
		EVERYTHING,
		FONT
	};

	//Return Internal Path Function
	static std::string Location(std::string name, std::string pos = "");

	//Json File Parsing
	//static Json::Value GetJsonFile(std::string path);

	//Open Files and Get Absolute Path as string
	static std::string OpenFileName(FileType file_type);

	//Open Binary File as String
	static std::string OpenFile(std::string path);

	//Save File as String
	static int SaveFile(std::string path, std::string value);

	//Save File as JSON
	//static int SaveFile(std::string path, Json::Value& value);
};