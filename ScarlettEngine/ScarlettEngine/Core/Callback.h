#pragma once

namespace Scarlett
{

template <typename T, typename ...Args>
class Callback
{
public:
    inline static Callback<T, Args ...> CreateCallback(std::function<T (Args...)> function)
    {
        Callback<T, Args...> callback;
        callback.mFunction = function;
        return callback;
    }

    inline T operator () (Args ...args) const
    {
        return mFunction(args...);
    }

private:
    std::function < T(Args...)> mFunction;
};

} // Namespace Scarlett.