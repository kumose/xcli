set(CLI_EXAMPLES
    minimal simple subcommands option_groups enum validators
    custom_validator formatter config_app ranges
)

foreach(E ${CLI_EXAMPLES})
    kmcmake_cc_binary(
        NAME ${E}
        SOURCES cli/${E}.cc
        DEPS ${PROJECT_NAME}::cli
        CXXOPTS ${KMCMAKE_CXX_OPTIONS}
        LINKS ${KMCMAKE_DEPS_LINK}
    )
endforeach()
