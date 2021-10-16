project(docs)

find_package(doxygen-cmake REQUIRED)

log_level(_doxygen_list_inputs DEBUG)
# set(dot.class-diagrams false)
if (TARGET Doxygen::doxygen)
    doxygen_add_doc_targets(PROJECT_FILE Doxyfile EXAMPLE_DIRECTORIES example GENERATE_PDF)
else()
    message(STATUS "No Doxygen found.")
endif()
