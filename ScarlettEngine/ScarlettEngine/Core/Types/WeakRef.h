#pragma once

namespace Scarlett
{

/**
 * @class WeakRef: A class used to hold a reference to a pointer such that the ownership is not transferred.<br/>
 * Having a \c Ref to a pointer allows you to access the pointer and its methods but not call \c delete on it.
 * @tparam T: The class of the pointer.
 */
template <typename T>
class WeakRef
{
public:
    WeakRef()      = default;
    ~WeakRef()     = default;

    WeakRef(const WeakRef&)             = default;
    WeakRef(WeakRef&&)                  = delete;
    WeakRef& operator=(WeakRef&&)       = default;
    WeakRef& operator=(const WeakRef&)  = default;

    inline explicit WeakRef(T* reference)
        : mReferencePtr(reference)
    { /* Empty. */ }

    /**
     * A method used to check if the pointer is still valid.
     * @return \c true if the reference is still valid, \c false otherwise.
     */
    inline bool IsRefValid() const { return mReferencePtr != nullptr; }
    /**
     * A method used to invalidate a pointer.
     */
    inline void Invalidate() { mReferencePtr = nullptr;  }

    inline T* GetRawPtr() const { return mReferencePtr; }

    inline T* operator->()              { return mReferencePtr; }
    inline const T* operator->() const  { return mReferencePtr; }
private:
    T* mReferencePtr = nullptr;
};

} // Namespace Scarlett.
