#pragma once

//todo need to move this and change the hardcoded 50. (Should this be a string stl implementation or just for tag component?
namespace ScarlettStl
{

class String
{
public:
    explicit String(const std::string& str)
    {
        *this = str;
    }

    [[nodiscard]] const char* c_str() const { return mStr; }

    String& operator=(const std::string& rhs)
    {
        const int length = std::min(static_cast<int>(rhs.length()), 50);
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
