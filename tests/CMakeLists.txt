add_executable(ensure_utf8 cli/ensure_utf8.cpp)
add_executable(ensure_utf8_twice cli/ensure_utf8_twice.cpp)
target_link_libraries(ensure_utf8 PRIVATE ${PROJECT_NAME}::cli)
target_link_libraries(ensure_utf8_twice PRIVATE ${PROJECT_NAME}::cli)

set(CLI_TESTS
    simple_test app_test helpers_test creation_test subcommand_test
    option_type_test numeric_type_test set_test transform_test help_test
    formatter_test new_parse_test config_file_test optional_test
    string_parse_test complex_type_test true_false_test locale_test
    option_group_test extra_validators_test encoding_test
    flags_test
)

foreach(T ${CLI_TESTS})
    kmcmake_cc_test(
        NAME ${T}
        MODULE cli
        SOURCES
            cli/${T}.cc
            cli/main.cc
        DEPS ${PROJECT_NAME}::cli
        CXXOPTS ${KMCMAKE_CXX_OPTIONS}
    )
    if(${T} STREQUAL "app_test")
        target_compile_definitions(cli_${T} PRIVATE
            XCLI_ENSURE_UTF8_EXE="$<TARGET_FILE:ensure_utf8>"
            XCLI_ENSURE_UTF8_TWICE_EXE="$<TARGET_FILE:ensure_utf8_twice>"
        )
        target_link_libraries(cli_${T} PRIVATE Threads::Threads)
        add_dependencies(cli_${T} ensure_utf8 ensure_utf8_twice)
    endif()
endforeach()

kmcmake_cc_test(
    NAME flags_declare_test
    MODULE cli
    SOURCES
        cli/flags_declare_test.cc
        cli/flags_declare_flags.cc
        cli/main.cc
    DEPS ${PROJECT_NAME}::cli
    CXXOPTS ${KMCMAKE_CXX_OPTIONS}
)