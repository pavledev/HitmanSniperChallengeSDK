#pragma once

#define REF_IID_PPV_ARGS(Val) IID_PPV_ARGS(&Val.ref)

template <class T>
struct ScopedD3DRef
{
    typedef T* RefType;

    ScopedD3DRef(RefType ref) : ref(ref) {}

    ScopedD3DRef() : ref(nullptr) {}

    ScopedD3DRef(const ScopedD3DRef& other)
    {
        ref = other.ref;

        if (ref)
            ref->AddRef();

        return *this;
    }

    ScopedD3DRef(ScopedD3DRef&& other) noexcept
    {
        ref = other.ref;
        other.ref = nullptr;
    }

    ~ScopedD3DRef()
    {
        if (ref)
            ref->Release();
    }

    ScopedD3DRef& operator=(const ScopedD3DRef& other)
    {
        Reset();
        ref = other.ref;

        if (ref)
            ref->AddRef();

        return *this;
    }

    ScopedD3DRef& operator=(ScopedD3DRef&& other) noexcept
    {
        ref = other.ref;
        other.ref = nullptr;
        return *this;
    }

    ScopedD3DRef& operator=(RefType ref)
    {
        Reset();
        this->ref = ref;

        if (this->ref)
            this->ref->AddRef();

        return *this;
    }

    void Reset()
    {
        if (ref)
            ref->Release();

        ref = nullptr;
    }

    RefType operator->() const
    {
        return ref;
    }

    operator RefType() const
    {
        return ref;
    }

    operator bool() const
    {
        return ref != nullptr;
    }

    void* VTable() const
    {
        return *reinterpret_cast<void**>(ref);
    }

    RefType* ReleaseAndGetPtr()
    {
        Reset();
        return &ref;
    }

    RefType ref;
};

inline void BreakIfFailed(HRESULT result)
{
    if (FAILED(result))
        DebugBreak();
}

struct SafeHandle
{
    SafeHandle(HANDLE p_Handle) : handle(handle) {}

    SafeHandle() : handle(nullptr) {}

    SafeHandle(const SafeHandle&) = delete;

    SafeHandle(SafeHandle&& other) noexcept
    {
        handle = other.handle;
        other.handle = nullptr;
    }

    ~SafeHandle()
    {
        if (handle)
            CloseHandle(handle);
    }

    SafeHandle& operator=(const SafeHandle&) = delete;

    SafeHandle& operator=(SafeHandle&& other) noexcept
    {
        handle = other.handle;
        other.handle = nullptr;
        return *this;
    }

    SafeHandle& operator=(HANDLE handle)
    {
        Reset();
        this->handle = handle;
        return *this;
    }

    void Reset()
    {
        if (handle)
            CloseHandle(handle);

        handle = nullptr;
    }

    operator bool() const
    {
        return handle != nullptr;
    }

    HANDLE* ReleaseAndGetPtr()
    {
        Reset();
        return &handle;
    }

    HANDLE handle = nullptr;
};
