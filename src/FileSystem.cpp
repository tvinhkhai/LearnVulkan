#include "stdafx.h"
#include "FileSystem.h"

#include <fstream>

///////////////////////////////////////////////////////////////////////////////

FileSystem::FileSystem()
{
}

///////////////////////////////////////////////////////////////////////////////

FileSystem::~FileSystem()
{
}

///////////////////////////////////////////////////////////////////////////////

std::vector<char> FileSystem::readFile(const std::string& i_fileName)
{
    std::ifstream file(i_fileName, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

///////////////////////////////////////////////////////////////////////////////