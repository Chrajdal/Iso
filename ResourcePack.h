#pragma once

#include <streambuf>
#include <map>
#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <chrono>
#include <vector>
#include <list>
#include <fstream>
#include <functional>
#include <algorithm>
#include "olcArtifacts.h"

namespace olc {
	class ResourcePack
	{
	public:
		ResourcePack();
		~ResourcePack();
		struct sEntry : public std::streambuf {
			uint32_t nID = 0, nFileOffset = 0, nFileSize = 0; uint8_t* data = nullptr; void _config() { this->setg((char*)data, (char*)data, (char*)(data + nFileSize)); }
		};

	public:
		olc::rcode AddToPack(std::string sFile);

	public:
		olc::rcode SavePack(std::string sFile);
		olc::rcode LoadPack(std::string sFile);
		olc::rcode ClearPack();

	public:
		olc::ResourcePack::sEntry GetStreamBuffer(std::string sFile);

	private:

		std::map<std::string, sEntry> mapFiles;
	};
}