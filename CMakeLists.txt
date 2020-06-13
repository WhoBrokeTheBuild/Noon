CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "$ENV{CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

MESSAGE(STATUS "${CMAKE_MODULE_PATH}")

INCLUDE(Policies)

###
### Project
###

PROJECT(Noon)

###
### Options
###

OPTION(VERBOSE_LOGGING "Build with Verbose Logging enabled" OFF)

OPTION(BUILD_DEMO "Build Demo" ON)

OPTION(BUILD_TESTS "Build Tests" OFF)

###
### Configuration
###

INCLUDE(Compiler)

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

###
### Dependencies
###

FIND_PACKAGE(SFML COMPONENTS system window graphics REQUIRED)

IF(NOT TARGET SFML::SFML)
    ADD_LIBRARY(SFML::SFML INTERFACE IMPORTED)
    SET_TARGET_PROPERTIES(
        SFML::SFML PROPERTIES
        # Use SFML_LIBRARIES and SFML_INCLUDE_DIR for the FindSFML.cmake variant
        INTERFACE_LINK_LIBRARIES "${SFML_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${SFML_INCLUDE_DIR}"
    )

    # Link the targets for the SFMLConfig.cmake variant
    IF(TARGET sfml-system)
        TARGET_LINK_LIBRARIES(SFML::SFML INTERFACE sfml-system)
    ENDIF()
    IF(TARGET sfml-window)
        TARGET_LINK_LIBRARIES(SFML::SFML INTERFACE sfml-window)
    ENDIF()
    IF(TARGET sfml-graphics)
        TARGET_LINK_LIBRARIES(SFML::SFML INTERFACE sfml-graphics)
    ENDIF()
ENDIF()

FIND_PACKAGE(Python3 3.8 COMPONENTS Development REQUIRED)

###
### PreProcessor
###

ADD_SUBDIRECTORY(util/preproc)

###
### Engine
###

ADD_SUBDIRECTORY(engine)

###
### Editor
###

ADD_SUBDIRECTORY(editor)

###
### Demo
###

IF(BUILD_DEMO)
    ADD_SUBDIRECTORY(demo)
ENDIF()
