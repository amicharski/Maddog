cmake_minimum_required(VERSION 3.20)
project(MaddogCompiler C)

set(CMAKE_C_STANDARD 11)

add_executable(MaddogCompiler main.c core/tokenizer.c core/parser.c core/shell.c)
