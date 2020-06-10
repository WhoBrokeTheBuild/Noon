
ADD_EXECUTABLE(NoonPreprocessor "")

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
    NoonPreprocessor
    PRIVATE
        ${_PRIVATE}
)

TARGET_INCLUDE_DIRECTORIES(
    NoonPreprocessor
    PRIVATE
        private
)