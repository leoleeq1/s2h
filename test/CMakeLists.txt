set (VECTOR_TEST_SOURCE
    vector_success.cpp
)

set (MATRIX_TEST_SOURCE
    matrix_success.cpp
)

create_test_sourcelist (MathTestsList MathTests.cpp 
    ${VECTOR_TEST_SOURCE}
    ${MATRIX_TEST_SOURCE}
)

add_executable (MathTests ${MathTestsList})
target_link_libraries(
    MathTests
    PRIVATE s2h_library
)
target_compile_features(MathTests PRIVATE cxx_std_23)

# add_executable (successTest vector.success.cpp)
# target_link_libraries(
#     successTest
#     PRIVATE s2h_library
# )
# target_compile_features(MathTests PRIVATE cxx_std_23)
# add_test(NAME vectorSuccess COMMAND successTest)

list(REMOVE_ITEM MathTestsList MathTests.cpp)

foreach (test ${MathTestsList})
    get_filename_component (TName ${test} NAME_WE)
    add_test (NAME ${TName} COMMAND MathTests ${TName})
    if (${TName} MATCHES "(vector|matrix)_fail\..+")
        set_property(TEST ${TName} WILL_FAIL true)
    endif()
endforeach ()
