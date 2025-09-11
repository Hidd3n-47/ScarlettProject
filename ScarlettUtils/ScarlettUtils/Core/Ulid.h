#pragma once

#include <scarlettTypes/Types.h>

namespace ScarlettUtils
{

class Ulid
{
public:
    Ulid();
    constexpr explicit Ulid(const uint64 ulid) : mUlid{ ulid } { /* Empty. */ }

    inline operator uint64() const
    {
        return mUlid;
    }
private:
    uint64 mUlid;

    [[nodiscard]] static uint32 GetTimeFromEpoch();

    [[nodiscard]] static uint32 GetRandomNumber();// { return mUniformDistribution(mRandomEngine); }
};

} // Namespace Scarlett.

template <>
struct std::hash<ScarlettUtils::Ulid>
{
    [[nodiscard]] inline size_t operator()(const ScarlettUtils::Ulid& ulid) const noexcept
    {
        return std::hash<uint64>()(ulid);
    }
};