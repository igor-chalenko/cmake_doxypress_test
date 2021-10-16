# Copyright (c) 2018 Igor Chalenko
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.1)
project(ctaeb
        VERSION 1.0
        DESCRIPTION "Compile-time algebraic expressions"
        LANGUAGES CXX)

# Top-level build description for cmake
set(CMAKE_CXX_STANDARD 17)
# set(PROJECT_LINK_LIBRARIES pthread)

add_library(ctaeb INTERFACE)
target_include_directories(ctaeb INTERFACE include)

set(SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/include/ctaeb/expression.h
        ${PROJECT_SOURCE_DIR}/include/ctaeb/operations.h
        ${PROJECT_SOURCE_DIR}/include/ctaeb/print.h
        ${PROJECT_SOURCE_DIR}/include/ctaeb/ctaeb.h)

target_sources(ctaeb INTERFACE ${SOURCE_FILES})

add_executable(ctaeb.compound-example
        EXCLUDE_FROM_ALL example/compound.cc)
target_link_libraries(ctaeb.compound-example ctaeb)

add_subdirectory(doc)