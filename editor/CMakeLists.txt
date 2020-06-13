
ADD_LIBRARY(NoonEditor SHARED "")

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

TARGET_SOURCES(
    NoonEditor
    PRIVATE
        ${_PUBLIC}
        ${_PRIVATE}
        ${_GENERATED_HEADER}
)

FOREACH(file ${_PUBLIC} ${_PRIVATE})
    FILE(RELATIVE_PATH file_path ${CMAKE_CURRENT_SOURCE_DIR} ${file})
    GET_FILENAME_COMPONENT(folder ${file_path} DIRECTORY)
    FILE(TO_NATIVE_PATH ${folder} folder)
    SOURCE_GROUP(${folder} FILES ${file})
ENDFOREACH()

TARGET_INCLUDE_DIRECTORIES(
    NoonEditor
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    PRIVATE
        private
)

TARGET_LINK_LIBRARIES(
    NoonEditor
    NoonEngine
)

TARGET_COMPILE_DEFINITIONS(
    NoonEditor
    PRIVATE
        # Disable Visual Studio "not secure" warnings
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

TARGET_COMPILE_OPTIONS(
    NoonEditor
    PRIVATE
        # Disable unknown pragmas warning, C++ exceptions
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas -fno-exceptions>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas -fno-exceptions>
        $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc- /GR->
)

TARGET_COMPILE_FEATURES(
    NoonEditor
    PRIVATE
        cxx_std_17
)

SET_TARGET_PROPERTIES(
    NoonEditor
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)
