
TARGET_INCLUDE_DIRECTORIES(
    ${_ENGINE_TARGET} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

FILE(GLOB_RECURSE _ENGINE_INCLUDES RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR}/inc/*.hpp ${CMAKE_CURRENT_LIST_DIR}/inc/*.h)
FILE(GLOB_RECURSE _ENGINE_SOURCES  RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

SET(_ENGINE_GENERATED_SOURCE ${CMAKE_BINARY_DIR}/Engine.gen.hpp)
ADD_CUSTOM_COMMAND(
    OUTPUT ${_ENGINE_GENERATED_SOURCE}
    COMMAND ${_PREPROC_TARGET} ${_ENGINE_GENERATED_SOURCE} ${_ENGINE_INCLUDES} ${_ENGINE_SOURCES}
    DEPENDS ${_ENGINE_INCLUDES} ${_ENGINE_SOURCES} ${_PREPROC_TARGET}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

TARGET_SOURCES(
    ${_ENGINE_TARGET} PRIVATE
    ${_ENGINE_INCLUDES}
    ${_ENGINE_SOURCES}
    ${_ENGINE_GENERATED_SOURCE}
)

# SFML
SET(SFML_DIR "${CMAKE_BINARY_DIR}/depend/SFML")

ExternalProject_Add(
    depend-sfml
    GIT_REPOSITORY    "https://github.com/SFML/SFML"
    GIT_TAG           "2.5.x"
    GIT_SHALLOW       TRUE
    SOURCE_DIR        "${SFML_DIR}"
    BUILD_IN_SOURCE   TRUE
    CMAKE_ARGS        -DSFML_BUILD_AUDIO=OFF
                      -DSFML_BUILD_NETWORK=OFF
                      -DSFML_BUILD_EXAMPLES=OFF
                      -DSFML_BUILD_DOC=OFF
                      -DSFML_INSTALL_PKGCONFIG_FILES=OFF
					  -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
					  -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
                      -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
    UPDATE_COMMAND    ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
)

ADD_DEPENDENCIES(
    ${_ENGINE_TARGET}
    depend-sfml
)

TARGET_INCLUDE_DIRECTORIES(
    ${_ENGINE_TARGET} PUBLIC
    ${CMAKE_BINARY_DIR}
    ${SFML_DIR}/include
)

IF(WIN32)
    TARGET_LINK_LIBRARIES(
        ${_ENGINE_TARGET}
		debug sfml-graphics-d
		debug sfml-system-d
		debug sfml-window-d
		optimized sfml-graphics
		optimized sfml-system
		optimized sfml-window
	)
ELSE()
    TARGET_LINK_LIBRARIES(
        ${_ENGINE_TARGET}
		sfml-graphics
		sfml-system
		sfml-window
	)
ENDIF()

# Python
SET(PYTHON_DIR "${CMAKE_BINARY_DIR}/depend/cpython")

SET(PYTHON_CONFIGURE_COMMAND "")
SET(PYTHON_BUILD_COMMAND "")

IF(WIN32)
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(ARCH "x64")
    ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(ARCH "Win32")
    ENDIF()
    SET(PYTHON_CONFIGURE_COMMAND "")
    SET(PYTHON_BUILD_COMMAND1 cmd /c PCbuild\\build.bat -c Debug -t Build -p ${ARCH})
    SET(PYTHON_BUILD_COMMAND2 cmd /c PCbuild\\build.bat -c Release -t Build -p ${ARCH})
ELSE()
    SET(PYTHON_CONFIGURE_COMMAND ./configure --enable-shared)
    SET(PYTHON_BUILD_COMMAND1 make)
ENDIF()

ExternalProject_Add(
    depend-python
    GIT_REPOSITORY    "https://github.com/python/cpython"
    GIT_TAG           "3.7"
    GIT_SHALLOW       TRUE
    SOURCE_DIR        "${PYTHON_DIR}"
    BUILD_IN_SOURCE   TRUE
    CONFIGURE_COMMAND "${PYTHON_CONFIGURE_COMMAND}"
    BUILD_COMMAND     "${PYTHON_BUILD_COMMAND1}"
    COMMAND           "${PYTHON_BUILD_COMMAND2}"
    UPDATE_COMMAND    ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
)

ADD_DEPENDENCIES(
    ${_ENGINE_TARGET}
    depend-python
)

TARGET_INCLUDE_DIRECTORIES(
    ${_ENGINE_TARGET} PUBLIC
    ${PYTHON_DIR}
    ${PYTHON_DIR}/Include
)

IF(WIN32)
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(ARCH "amd64")
    ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(ARCH "win32")
    ENDIF()
    TARGET_LINK_LIBRARIES(
        ${_ENGINE_TARGET}
        debug python37_d.lib
        optimized python37.lib
    )
    ADD_CUSTOM_TARGET(depend-python-copy
        COMMAND ${CMAKE_COMMAND} -E copy ${PYTHON_DIR}/PCbuild/${ARCH}/python37_d.lib ${CMAKE_BINARY_DIR}/$(Configuration)/python37_d.lib
        COMMAND ${CMAKE_COMMAND} -E copy ${PYTHON_DIR}/PCbuild/${ARCH}/python37.lib ${CMAKE_BINARY_DIR}/$(Configuration)/python37.lib
        COMMAND ${CMAKE_COMMAND} -E copy ${PYTHON_DIR}/PCbuild/${ARCH}/python37_d.dll ${CMAKE_BINARY_DIR}/$(Configuration)/python37_d.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${PYTHON_DIR}/PCbuild/${ARCH}/python37.dll ${CMAKE_BINARY_DIR}/$(Configuration)/python37.dll
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PYTHON_DIR}/Lib ${CMAKE_BINARY_DIR}/$(Configuration)/Lib
    )
    ADD_DEPENDENCIES(
        ${_ENGINE_TARGET}
        depend-python-copy
    )
ELSE()
    TARGET_LINK_LIBRARIES(
        ${_ENGINE_TARGET}
        ${PYTHON_DIR}/libpython3.7m.so
	)
ENDIF()
