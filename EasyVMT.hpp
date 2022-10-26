#pragma once

#include <cstdint>

class EasyVMT 
{
private:
    uintptr_t* vmt;
public:
    uintptr_t** vTableAddress = nullptr;
    uintptr_t* originalAddress = nullptr;
    size_t virtualClassFunctionCount = 0;

    // ----------------------------------------------------------------------------------------------------------------------

    inline unsigned int GetVirtualFunction(void* virtualClass, unsigned int virtualIndex)
    {
        return static_cast<unsigned int>((*static_cast<int**>(virtualClass))[virtualIndex]);
    }

    // ----------------------------------------------------------------------------------------------------------------------

    inline EasyVMT(void* virtualClass) 
    {
        vTableAddress = reinterpret_cast<uintptr_t**>(virtualClass);

        // Acquire the amount of functions inside the virtual class.
        while (reinterpret_cast<uintptr_t*>(*vTableAddress)[virtualClassFunctionCount]) 
        {
            virtualClassFunctionCount++;
        }

        // Set the original.
        originalAddress = *vTableAddress;

        vmt = new uintptr_t[virtualClassFunctionCount + 1];

        // Make a copy of the entire vTable.
        memcpy(vmt, &originalAddress[-1], (sizeof(uintptr_t) * virtualClassFunctionCount) + sizeof(uintptr_t));
    }

    // ----------------------------------------------------------------------------------------------------------------------

    inline void Hook(void* detourFunction, size_t functionIndex) 
    {
        // A simple sanity check to ensure that are hook is valid and in valid boundaries.
        bool isValidHook = detourFunction != nullptr && functionIndex >= 0 && functionIndex <= virtualClassFunctionCount;

        if (isValidHook)
        {
            vmt[functionIndex + 1] = reinterpret_cast<uintptr_t>(detourFunction);
            *vTableAddress = &vmt[1];
        }
    }

    // ----------------------------------------------------------------------------------------------------------------------

    inline void Unhook() 
    {
        *vTableAddress = originalAddress;
        delete vmt;
    }

    // ----------------------------------------------------------------------------------------------------------------------

    inline uintptr_t* GetOriginalAddress() 
    {
        if (originalAddress != nullptr)
            return originalAddress;
    }

    // ----------------------------------------------------------------------------------------------------------------------

    template<typename Fn>
    inline Fn GetOriginalMethod(size_t methodIndex) 
    {
        return reinterpret_cast<Fn>(originalAddress[methodIndex]);
    }

    // ----------------------------------------------------------------------------------------------------------------------
};
