#include "FileIO.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "Config.h"
#include "Log.h"
#include "FileIO.h"
#include <filesystem>
#include <ProtableFileDialogs.h> //For Open Files with Multiplatforms

using namespace std;

std::string FileIO::Location(std::string name, std::string pos) {
	std::string loc;
	loc = name;
	if (pos != "") {
		loc = pos + "\\" + loc;
	}
	if (RELEASE_BUILD) {
		return (std::string)getenv("APPDATA") + "\\" + "Flow" + "\\" + loc;
	}
	else {
		return "Resources\\" + loc;
	}
}

//Json::Value FileIO::GetJsonFile(std::string path) {
//	Log::Debug("FileIO", "GetJsonFile", "Path : ", path);
//	Json::Value root;
//	Json::CharReaderBuilder reader;
//	ifstream is(path, ifstream::binary);
//	string errorMessage;
//	auto bret = Json::parseFromStream(reader, is, &root, &errorMessage);
//
//	if (bret == false) {
//		Log::DebugFree("Error to parse JSON file !!!");
//		Log::DebugFree("Details :", errorMessage);
//		Log::Error(L"Error to parse JSON file - Fatal");
//	}
//
//	return root;
//}

std::string FileIO::OpenFileName(FileType file_type) {
	vector<string> selection;
	switch (file_type) {
	case FileType::EVERYTHING:
		selection = pfd::open_file("Open File", "", { "All Files", "*" }, pfd::opt::force_path).result();
		break;
	case FileType::FONT:
		selection = pfd::open_file("Open File", "", { "Font Files", "*.ttf OR *.otf" }, pfd::opt::force_path).result();
		break;
	case FileType::TYPO:
		selection = pfd::open_file("Open File", "", { "Typo Files", "*.typo" }, pfd::opt::force_path).result();
		break;
	}
		
	if (selection.empty()) {
		Log::Debug("FileIO", "OpenFileName", "Connot open file - Not selected");
		return "";
	}
	Log::Debug("FileIO", "OpenFileName", "File : ", selection.front());
	for (int i = 0; i < selection.front().size(); i++) {
		if (selection.front()[i] < 0) {
			Log::Error(L"Root path should contain only ASCII characters. - Fatal");
		}
	}
	return selection.front();
}

char* ConvertTCharToChar(const TCHAR* tcharString, UINT codePage)
{
	int charCount = WideCharToMultiByte(codePage, 0, tcharString, -1, NULL, 0, NULL, NULL);
	char* charString = new char[charCount];
	WideCharToMultiByte(codePage, 0, tcharString, -1, charString, charCount, NULL, NULL);
	return charString;
}

std::string FileIO::OpenFile(std::string path) {
	FILE* file;
	fopen_s(&file, path.c_str(), "rb");
	string data_bin = "";
	while (feof(file) == 0)
		data_bin += fgetc(file);
	fclose(file);
	data_bin.pop_back();
	return data_bin;
}

int FileIO::SaveFile(std::string path, std::string value) {
	FILE* f;
	fopen_s(&f, path.c_str(), "wb");
	for (int i = 0; i < value.size(); i++) {
		fputc(value[i], f);
	}
	fclose(f);
	return 0;
}

//int FileIO::SaveFile(std::string path, Json::Value& value) {
//	Json::StreamWriterBuilder builder;
//	builder.settings_["emitUTF8"] = true;
//	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
//	std::ofstream outputFileStream(path);
//	writer->write(value, &outputFileStream);
//	return 0;
//}