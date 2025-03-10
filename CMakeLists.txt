cmake_minimum_required(VERSION 3.12.0)
project(Tetsaw)

# By configuring CMake with -DDOWNLOAD_DEPENDENCIES=ON/OFF,
# users can choose between downloading dependencies or using system libraries
option(DOWNLOAD_DEPENDENCIES "Download dependencies" TRUE)

if (DOWNLOAD_DEPENDENCIES)
	# FetchContent downloads and configures dependencies
	include(FetchContent)
	FetchContent_Declare(
			SDL3
			GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
			GIT_TAG "main"
			EXCLUDE_FROM_ALL
	)
	FetchContent_MakeAvailable(SDL3)
else ()
	# find_package looks for already-installed system packages.
	# Configure with `-DCMAKE_PREFIX_PATH="/path/to/package1;/path/to/package2"`
	# to add search paths.
	find_package(SDL3 CONFIG REQUIRED)
endif ()

if (ANDROID)
	# SDL applications need to be built as a shared library
	function(add_executable TARGET)
		add_library(${TARGET} SHARED ${ARGN})
	endfunction()
endif ()

add_executable(${CMAKE_PROJECT_NAME}
		src/main.c
		src/cell.c
		src/draw_utils.c
		src/board.c
		src/myappstate.c
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3)

# This is safe to set on all platforms. Otherwise your SDL app will
#  have a terminal window pop up with it on Windows.
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY WIN32_EXECUTABLE TRUE)

if (EMSCRIPTEN)
	# Output to index.html for easier deployments
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
	# Create a html webpage
	set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY SUFFIX ".html")
	# Use custom emscripten shell
	target_link_options(${CMAKE_PROJECT_NAME}
			PRIVATE
			--shell-file ${CMAKE_SOURCE_DIR}/emscripten_shell.html
	)
endif ()
