project(docs)

find_package(doxygen-cmake REQUIRED)

log_level(doxygen DEBUG)
# set(dot.class-diagrams false)
if (TARGET Doxygen::doxygen)
    add_doxygen_targets(PROJECT_FILE Doxyfile EXAMPLE_DIRECTORIES example GENERATE_PDF)
else()
    message(STATUS "No Doxygen found.")
endif()
