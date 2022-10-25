# VMT
A header only easy to use VMT hooking class.

Example usage:

```javascript

unsigned int GetVirtualFunction(void* virtualClass, unsigned int virtualIndex)
{
    return static_cast<unsigned int>((*static_cast<int**>(virtualClass))[virtualIndex]);
}
    
int main()
{
   CVMT* virtualMethodHook = new CVMT(dummyClass);
   
   void* addr = reinterpret_cast<void*>(GetVirtualFunction(yourClass, yourIndex));
   
   yourOriginal = (YourOriginalFn)addr;
   
   virtualMethodHook->Hook(YourDetourFunction, virtualIndex);
}
```
# To Do
- Update code.
- Make it as stealth as possible.
