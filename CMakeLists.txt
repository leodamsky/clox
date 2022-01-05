cmake_minimum_required(VERSION 3.21)
project(clox C)

set(CMAKE_C_STANDARD 23)

add_executable(clox src/main.c src/chunk.c src/memory.c src/debug.c src/value.c src/vm.c src/compiler.c src/scanner.c)
