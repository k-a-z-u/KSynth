# Usage:
#  Create build folder, like RC-build next to RobotControl and WifiScan folder
#  CD into build folder and execute 'cmake -DCMAKE_BUILD_TYPE=Debug ../RobotControl'
#  make

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# select build type
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" )

PROJECT(KSynth_Tests)

IF(NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "No build type selected. Default to Debug")
	SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()


# all supported options:
#option(WITH_ZLIB "Build with zlib compression support" OFF)
#MESSAGE(STATUS "zlib compression support (WITH_ZLIB):			${WITH_ZLIB}")
#IF(WITH_ZLIB)
#	add_definitions(-DWITH_ZLIB)
#	SET(EXTRA_LIBS ${EXTRA_LIBS} z)
#ENDIF()

INCLUDE_DIRECTORIES(
	../../
)


FILE(GLOB HEADERS
	./*.h
	./*/*.h
	./*/*/*.h
	./*/*/*/*.h
)

FILE(GLOB SOURCES
	./*.cpp
	./*/*.cpp
	./*/*/*.cpp
	./*/*/*/*.cpp
	${EXTRA_SOURCES}
)

ADD_DEFINITIONS(-DWITH_TESTS)

# system specific compiler flags
IF(MSVC)
	ADD_DEFINITIONS(-W3)
ELSE()
	ADD_DEFINITIONS(
		-std=c++11
		-Wall
		-Wextra
		-Wconversion
		-Werror=return-type
	)
ENDIF()



# build a binary file
ADD_EXECUTABLE(
	${PROJECT_NAME}
	${HEADERS}
	${SOURCES}
)

# needed external libraries
TARGET_LINK_LIBRARIES(
	${PROJECT_NAME}
	gtest
	pthread
	${EXTRA_LIBS}
)



