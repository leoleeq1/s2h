add_executable(s2h
    s2h.cpp
)

target_link_libraries(
    s2h
    PRIVATE s2h_library nw
)

target_compile_features(s2h PUBLIC cxx_std_23)