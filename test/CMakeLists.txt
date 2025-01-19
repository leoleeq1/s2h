set (VECTOR_TEST_SOURCE
    vector_success.cpp
)

create_test_sourcelist (VectorTestsList VectorTests.cpp ${VECTOR_TEST_SOURCE})

add_executable (VectorTests ${VectorTestsList})
target_link_libraries(
    VectorTests
    PRIVATE s2h_library
    )
target_compile_features(VectorTests PRIVATE cxx_std_23)

# add_executable (successTest vector.success.cpp)
# target_link_libraries(
#     successTest
#     PRIVATE s2h_library
# )
# target_compile_features(VectorTests PRIVATE cxx_std_23)
# add_test(NAME vectorSuccess COMMAND successTest)

foreach (test ${VECTOR_TEST_SOURCE})
    get_filename_component (TName ${test} NAME_WE)
    add_test (NAME ${TName} COMMAND VectorTests ${TName})
    if (${TName} MATCHES "vector_fail\..+")
        set_property(TEST ${TName} WILL_FAIL true)
    endif()
endforeach ()