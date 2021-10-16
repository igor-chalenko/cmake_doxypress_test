function(check_directory_exists name message out_var)
    message(STATUS "Current binary dir is ${CMAKE_CURRENT_BINARY_DIR}/${name}")
    if (NOT IS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${name})
        set(${out_var} false PARENT_SCOPE)
        list(APPEND messages ${message})
        set(messages ${messages} PARENT_SCOPE)
    endif()
endfunction()

function(check_file_exists name message out_var)
    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${name})
        set(${out_var} false PARENT_SCOPE)
        list(APPEND messages ${message})
        set(messages ${messages} PARENT_SCOPE)
    endif()
endfunction()

set(passed true)
set(messages "")

#if (NOT DOXYGEN_DOXYFILE STREQUAL "${PROJECT_SOURCE_DIR}/doc/Doxygen.in")
#    list(APPEND messages "Doxyfile not found")
#endif()

if (NOT DOXYPRESS_EXAMPLE_PATH STREQUAL "${PROJECT_SOURCE_DIR}/example")
    list(APPEND messages "example directory is not ${PROJECT_SOURCE_DIR}/example")
endif()

if (NOT DOXYGEN_INPUT STREQUAL "${PROJECT_SOURCE_DIR}/include")
    list(APPEND messages "input directory is not ${PROJECT_SOURCE_DIR}/include")
endif()


check_directory_exists(docs-generated "output directory not found" passed)
check_directory_exists(docs-generated/html "HTML output directory not found" passed)
#check_directory_exists(docs-generated/latex "LaTex output directory not found" passed)
check_file_exists(docs-generated/html/index.html "`index.html` not found" passed)
check_file_exists(docs-generated/html/classes.html "`classes.html` not found" passed)
if (passed)
    message(STATUS "Tests pass.")
else()
    #message(FATAL_ERROR ${messages})
endif()

