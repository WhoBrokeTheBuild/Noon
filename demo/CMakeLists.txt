
ADD_EXECUTABLE(NoonDemo "")
ADD_EXECUTABLE(NoonDemoEditor "")

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
    NoonDemo
    PRIVATE
        ${_PRIVATE}
)

TARGET_SOURCES(
    NoonDemoEditor
    PRIVATE
        ${_PRIVATE}
)

FOREACH(file ${_PRIVATE})
    FILE(RELATIVE_PATH file_path ${CMAKE_CURRENT_SOURCE_DIR} ${file})
    GET_FILENAME_COMPONENT(folder ${file_path} DIRECTORY)
    FILE(TO_NATIVE_PATH ${folder} folder)
    SOURCE_GROUP(${folder} FILES ${file})
ENDFOREACH()

TARGET_INCLUDE_DIRECTORIES(
    NoonDemo
    PRIVATE
        private
)

TARGET_INCLUDE_DIRECTORIES(
    NoonDemoEditor
    PRIVATE
        private
)

TARGET_LINK_LIBRARIES(
    NoonDemo
    NoonEngine
)

TARGET_LINK_LIBRARIES(
    NoonDemoEditor
    NoonEditor
)

TARGET_COMPILE_DEFINITIONS(
    NoonDemoEditor
    PUBLIC
        -DEDITOR_BUILD
)

ADD_CUSTOM_TARGET(
    run
    COMMAND $<TARGET_FILE:NoonDemo>
    DEPENDS NoonDemo
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_CUSTOM_TARGET(
    valgrind
    COMMAND valgrind $<TARGET_FILE:NoonDemo>
    DEPENDS NoonDemo
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_CUSTOM_TARGET(
    gdb
    COMMAND gdb --args $<TARGET_FILE:NoonDemo>
    DEPENDS NoonDemo
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_CUSTOM_TARGET(
    run-editor
    COMMAND $<TARGET_FILE:NoonDemoEditor>
    DEPENDS NoonDemoEditor
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

SET_PROPERTY(
    DIRECTORY ${CMAKE_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT "NoonDemo"
)

SET_TARGET_PROPERTIES(
    NoonDemo
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)