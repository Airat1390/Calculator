cmake_minimum_required(VERSION 3.10)
project(calculator)

include(FetchContent)

FetchContent_Declare(
    libmath
    GIT_REPOSITORY https://github.com/Airat1390/libmath.git    
    GIT_TAG v1.1.0
)
FetchContent_MakeAvailable(libmath)

find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)
endif()

add_executable(calculator
    src/app.cpp
    src/main.cpp
)

target_link_libraries(calculator
    PRIVATE
    libmath
)

install(TARGETS calculator
    RUNTIME DESTINATION bin
)

find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp src/*.h)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ALL_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

