file(GLOB HEADER_LIST CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(s2h
    "${HEADER_LIST}"
    "${SOURCE_LIST}"
)

target_link_libraries(
    s2h
    PRIVATE s2h_library nw
)

target_compile_features(s2h PUBLIC cxx_std_23)