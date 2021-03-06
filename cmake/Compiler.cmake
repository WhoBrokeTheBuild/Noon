
    SET(CMAKE_CXX_FLAGS "-std=c++14")
    ADD_DEFINITIONS(-D_XOPEN_SOURCE=700)
    ADD_COMPILE_OPTIONS(-Wall -Wextra)

# Configure Visual Studio exception handling
IF(MSVC)
    # Disable Default Exception Handling
    STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    # Disable Basic Runtime Checks
    STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS}")
    STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}")
ENDIF()
