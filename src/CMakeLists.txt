file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS
    "${s2h_SOURCE_DIR}/include/s2h/*.h"
)
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS
    "${s2h_SOURCE_DIR}/src/*.cpp"
)

add_library(s2h_library
    ${SOURCE_LIST}
    ${HEADER_LIST}
)

target_include_directories(s2h_library PUBLIC ../include)

target_compile_features(s2h_library PUBLIC cxx_std_23)

target_link_libraries(
    s2h_library
    PRIVATE nw
)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST}
)