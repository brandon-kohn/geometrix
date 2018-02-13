#include <gtest/gtest.h>

namespace testing { namespace internal
{
    enum GTestColor 
    {
        COLOR_DEFAULT,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_YELLOW
    };

    extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
    
    #define GEOMETRIX_PRINTF(...)  do { testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__); } while(0)
   
    // C++ stream interface
    class GeometrixTestCout : public std::stringstream
    {
    public:
        ~GeometrixTestCout()
        {
            GEOMETRIX_PRINTF("%s", str().c_str());
        }
    };
}}//! namespace::testing::internal;

#define GMESSAGE() ::testing::internal::GeometrixTestCout()
