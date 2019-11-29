#include "ResourcePack.h"

namespace olc {
	ResourcePack::ResourcePack()
	{

	}

	ResourcePack::~ResourcePack()
	{
		ClearPack();
	}

	olc::rcode ResourcePack::AddToPack(std::string sFile)
	{
		std::ifstream ifs(sFile, std::ifstream::binary);
		if (!ifs.is_open()) return olc::rcode::FAIL;

		// Get File Size
		std::streampos p = 0;
		p = ifs.tellg();
		ifs.seekg(0, std::ios::end);
		p = ifs.tellg() - p;
		ifs.seekg(0, std::ios::beg);

		// Create entry
		sEntry e;
		e.data = nullptr;
		e.nFileSize = (uint32_t)p;

		// Read file into memory
		e.data = new uint8_t[(uint32_t)e.nFileSize];
		ifs.read((char*)e.data, e.nFileSize);
		ifs.close();

		// Add To Map
		mapFiles[sFile] = e;
		return olc::rcode::OK;
	}

	olc::rcode ResourcePack::SavePack(std::string sFile)
	{
		std::ofstream ofs(sFile, std::ofstream::binary);
		if (!ofs.is_open()) return olc::rcode::FAIL;

		// 1) Write Map
		size_t nMapSize = mapFiles.size();
		ofs.write((char*)& nMapSize, sizeof(size_t));
		for (auto& e : mapFiles)
		{
			size_t nPathSize = e.first.size();
			ofs.write((char*)& nPathSize, sizeof(size_t));
			ofs.write(e.first.c_str(), nPathSize);
			ofs.write((char*)& e.second.nID, sizeof(uint32_t));
			ofs.write((char*)& e.second.nFileSize, sizeof(uint32_t));
			ofs.write((char*)& e.second.nFileOffset, sizeof(uint32_t));
		}

		// 2) Write Data
		std::streampos offset = ofs.tellp();
		for (auto& e : mapFiles)
		{
			e.second.nFileOffset = (uint32_t)offset;
			ofs.write((char*)e.second.data, e.second.nFileSize);
			offset += e.second.nFileSize;
		}

		// 3) Rewrite Map (it has been updated with offsets now)
		ofs.seekp(std::ios::beg);
		ofs.write((char*)& nMapSize, sizeof(size_t));
		for (auto& e : mapFiles)
		{
			size_t nPathSize = e.first.size();
			ofs.write((char*)& nPathSize, sizeof(size_t));
			ofs.write(e.first.c_str(), nPathSize);
			ofs.write((char*)& e.second.nID, sizeof(uint32_t));
			ofs.write((char*)& e.second.nFileSize, sizeof(uint32_t));
			ofs.write((char*)& e.second.nFileOffset, sizeof(uint32_t));
		}
		ofs.close();

		return olc::rcode::OK;
	}

	olc::rcode ResourcePack::LoadPack(std::string sFile)
	{
		std::ifstream ifs(sFile, std::ifstream::binary);
		if (!ifs.is_open()) return olc::rcode::FAIL;

		// 1) Read Map
		uint32_t nMapEntries;
		ifs.read((char*)& nMapEntries, sizeof(uint32_t));
		for (uint32_t i = 0; i < nMapEntries; i++)
		{
			uint32_t nFilePathSize = 0;
			ifs.read((char*)& nFilePathSize, sizeof(uint32_t));

			std::string sFileName(nFilePathSize, ' ');
			for (uint32_t j = 0; j < nFilePathSize; j++)
				sFileName[j] = ifs.get();

			sEntry e;
			e.data = nullptr;
			ifs.read((char*)& e.nID, sizeof(uint32_t));
			ifs.read((char*)& e.nFileSize, sizeof(uint32_t));
			ifs.read((char*)& e.nFileOffset, sizeof(uint32_t));
			mapFiles[sFileName] = e;
		}

		// 2) Read Data
		for (auto& e : mapFiles)
		{
			e.second.data = new uint8_t[(uint32_t)e.second.nFileSize];
			ifs.seekg(e.second.nFileOffset);
			ifs.read((char*)e.second.data, e.second.nFileSize);
			e.second._config();
		}

		ifs.close();
		return olc::rcode::OK;
	}

	olc::ResourcePack::sEntry ResourcePack::GetStreamBuffer(std::string sFile)
	{
		return mapFiles[sFile];
	}

	olc::rcode ResourcePack::ClearPack()
	{
		for (auto& e : mapFiles)
		{
			if (e.second.data != nullptr)
				delete[] e.second.data;
		}

		mapFiles.clear();
		return olc::rcode::OK;
	}
}