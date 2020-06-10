
ADD_LIBRARY(NoonEngine SHARED "")

FILE(
    GLOB_RECURSE
    _PUBLIC
    public/*.h
    public/*.hh
    public/*.hpp
    public/*.c
    public/*.cc
    public/*.cpp
)

FILE(
    GLOB_RECURSE
    _PRIVATE
    private/*.h
    private/*.hh
    private/*.hpp
    private/*.c
    private/*.cc
    private/*.cpp
)

SET(
    _GENERATED_HEADER
    ${CMAKE_CURRENT_BINARY_DIR}/Engine.gen.hpp
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${_GENERATED_HEADER}
    COMMAND $<TARGET_FILE:NoonPreprocessor> ${_GENERATED_HEADER} ${_PUBLIC}
    DEPENDS NoonPreprocessor
)

TARGET_SOURCES(
    NoonEngine
    PRIVATE
        ${_PUBLIC}
        ${_PRIVATE}
        ${_GENERATED_HEADER}
)

TARGET_INCLUDE_DIRECTORIES(
    NoonEngine
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        private
)

TARGET_LINK_LIBRARIES(
    NoonEngine
    PUBLIC
        SFML::SFML
        Python3::Python
)

TARGET_COMPILE_DEFINITIONS(
    NoonEngine
    PRIVATE
        # Disable Visual Studio "not secure" warnings
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>

)

TARGET_COMPILE_OPTIONS(
    NoonEngine
    PRIVATE
        # Disable unknown pragmas warning, C++ exceptions
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas -fno-exceptions>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas -fno-exceptions>
        $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc- /GR->
)

TARGET_COMPILE_FEATURES(
    NoonEngine
    PRIVATE
        cxx_std_17
)

SET_TARGET_PROPERTIES(
    NoonEngine
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)
