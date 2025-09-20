cmake_minimum_required(VERSION 3.10)
project(lab1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Опции компиляции
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Два отдельных исполняемых файла
add_executable(child child.c)
add_executable(parent parent.c)

# # Добавление тестов
# enable_testing()

# add_executable(tests test/tests_l1.c)
# target_link_libraries(tests ${CMAKE_PROJECT_NAME}_lib gtest_main)

# # Добавление тестов в тестовый набор
# add_test(NAME MyProjectTests COMMAND tests)