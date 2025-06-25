cmake_minimum_required(VERSION 3.28)
project(video)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск pkg-config
find_package(PkgConfig REQUIRED)

# Автоматическое подключение библиотек FFmpeg
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavformat libavcodec libavutil libswscale)

# Добавляем исполняемый файл
add_executable(video main.cpp)

# Линкуем найденные библиотеки в правильном порядке
target_link_libraries(video PRIVATE PkgConfig::FFMPEG)