cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project("dejavu_server")

# attempt to find drogon
find_package(Drogon)

# clone specified release of drogon from GitHub if drogon not installed
if(NOT Drogon_FOUND)
	include(FetchContent)
	FetchContent_Declare(drogon_git
		GIT_REPOSITORY "https://github.com/drogonframework/drogon.git"
		GIT_TAG "v1.7.1"
	)
endif()

set(PROJECT_SOURCES
	src/main.cpp
	src/GameSocket.cpp
	src/GameSocket.h
	src/PlayerInfo.cpp
	src/PlayerInfo.h
	src/Utils.cpp
	src/Utils.h
	src/PlayerNote.cpp
	src/PlayerNote.h
)

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCES}
)

set(STATIC_ROOT "static" CACHE PATH "directory to serve static files from")
target_compile_definitions(${PROJECT_NAME} PRIVATE STATIC_ROOT="${STATIC_ROOT}")

if(NOT Drogon_FOUND)
	# disable unused drogon stuff if building from source
	option(BUILD_CTL OFF)
	option(BUILD_EXAMPLES OFF)
	option(BUILD_ORM OFF)
	FetchContent_MakeAvailable(drogon_git)
endif()

if(Drogon_FOUND)
	target_link_libraries(${PROJECT_NAME} PRIVATE ${DROGON_LIBRARIES})
else()
	target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
endif()

# copy static files
if(EXISTS "${CMAKE_SOURCE_DIR}/static")
	file(COPY "static" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
	message(WARNING "static directory does not exist, will not attempt to copy")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY "static" DESTINATION extras)
