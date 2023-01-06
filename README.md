# EasyVMT
A safe header only easy to use VMT hooking class.

Note from 1.6.2023 - Changes are going to be made and this is going to be improved soon.

- Bypasses all detections from this project: https://github.com/altoid29/NoVMT

Example usage:

```javascript
#include <iostream>

class ExampleClass 
{
public:
    // Starts at 0.
    virtual void ExampleFunction(int a) 
    {
        std::cout << a << std::endl;
    }
};

typedef void(__stdcall* ExampleFunctionFn)(int);
inline ExampleFunctionFn originalExampleFunction = nullptr;

void __stdcall ExampleFunctionHook(int a) 
{
    return originalExampleFunction(9999999);
}

int main() 
{
    ExampleClass* Example = new ExampleClass();

    EasyVMT* VMT1 = new EasyVMT(Example);

    originalExampleFunction = (ExampleFunctionFn)VMT1->GetVirtualFunction(Example, 0);

    VMT1->Hook(ExampleFunctionHook, 0);

    Example->ExampleFunction(1337);

    VMT1->Unhook();

    Example->ExampleFunction(1337);

    system("pause");
}
