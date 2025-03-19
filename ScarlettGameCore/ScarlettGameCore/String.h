#pragma once

#include "ScarlettGameCore/Src/ScarlettGame.h"

//todo need to move this and change the hardcoded 50. (Should this be a string stl implementation or just for tag component?
namespace ScarlettStl
{

class SCARLETT_GAME_CORE_API String
{
public:
	String(const std::string& str)
	{
		*this = str;
	}

	const char* c_str() const { return mStr; }

	String& operator=(const std::string& rhs)
	{
		size_t length = std::min(rhs.length(), (size_t)50);
		for (int i{ 0 }; i < length; ++i)
		{
			mStr[i] = rhs[i];
		}

		return *this;
	}
private:
	char mStr[50] = { " " };
};

}