cmake_minimum_required(VERSION 3.10)
set(CMAKE_PREFIX_PATH "C:/Cprog/SFML-2.6.1") # путь к библиотеке
project(caveGenerator) # название проекта
set(target CMakeTarget.exe) # имя exe-файла программы
set(SFML_STATIC_LIBRARIES TRUE) # настройка – использовать статические библиотеки
# поиск библиотек CMake умеет искать очень многое, выбирать из разных вариантов (32-64, debug-release, stacic-shared и т.д)
find_package(SFML 2.6 COMPONENTS graphics window system  REQUIRED)
# установятся переменные
#file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)


set(SRC_FILES "main.cpp" ) # h-файлы и cpp файлы через пробел 
add_executable(${target} ${SRC_FILES} )

target_link_libraries(${target} sfml-graphics sfml-window sfml-system)
# конкретные имена lib-файлов выберет сам cmake на основании результатов поиска
target_include_directories(${target} PRIVATE ${SFML_INCLUDE_DIR})
# говорим, где искать заголовочные файлы