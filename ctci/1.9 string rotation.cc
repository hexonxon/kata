#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include <iostream>

extern bool isSubstring(const std::string& haystack, const std::string& needle)
{
    return haystack.find(needle) != std::string::npos;    
}

/* Check that s2 is a rotation of s1 */
bool isRotation(const std::string& src, const std::string& rot)
{
    if (src.size() != rot.size() || src.size() == 0) {
        return false;
    }
    
    return isSubstring(rot + rot, src);
}

int main()
{
    assert(isRotation("waterbottle", "erbottlewat"));
    assert(!isRotation("waterbottle", "rbottlewat"));
    assert(!isRotation("waterbottle", ""));
    return 0;
}
