cmake_minimum_required(VERSION 3.10)
project(calculator)

include(FetchContent)

# Repository: libmath
FetchContent_Declare(
    libmath
    GIT_REPOSITORY https://github.com/Airat1390/libmath.git
    GIT_TAG main
)
FetchContent_MakeAvailable(libmath)

# Repository: clang-format-demo
FetchContent_Declare(
    clang_format_config
    URL https://github.com/ZlobinDA/clang-format-demo.git
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(clang_format_config)

# Repository: clang-tidy-demo
FetchContent_Declare(
    clang_tidy_config
    URL https://github.com/ZlobinDA/clang-tidy-demo.git
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(clang_tidy_config)

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

find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    # Явно указываем компилятору использовать скачанный файл .clang-tidy
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE} -config-file=${clang_tidy_config_SOURCE_DIR}/.clang-tidy)
endif()

find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp src/*.h)
    add_custom_target(format
        # Явно передаем путь к скачанному файлу .clang-format в команду
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file:${clang_format_config_SOURCE_DIR}/.clang-format ${ALL_SOURCE_FILES}
    )
endif()

