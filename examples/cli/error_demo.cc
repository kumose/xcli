// Demo: xcli error output format.
// Build: cmake -B build -DKMCMAKE_BUILD_EXAMPLES=ON && cmake --build build
// Run each demo by passing an argument:
//   ./error_demo duplicate   — OptionAlreadyAdded (duplicate --verbose)
//   ./error_demo excludes    — ExcludesError (--verbose vs --quiet)
//   ./error_demo required    — RequiredError (missing --output)

#include <xcli/cli.h>
#include <cstring>
#include <iostream>

int demo_duplicate() {
    xcli::App app("duplicate demo");
    bool v1{false}, v2{false};
    app.add_flag("--verbose", v1);
    app.add_flag("--verbose", v2);
    return 0;
}

int demo_excludes() {
    xcli::App app("excludes demo");
    bool verbose{false}, quiet{false};
    app.add_flag("--verbose,-V", verbose);
    app.add_flag("--quiet,-Q", quiet);
    app.get_option("--verbose")->excludes(app.get_option("--quiet"));
    char *argv[] = {(char*)"excludes", (char*)"--verbose", (char*)"--quiet", nullptr};
    int argc = 3;
    XCLI_PARSE(app, argc, argv);
    return 0;
}

int demo_required() {
    xcli::App app("required demo");
    std::string output;
    app.add_option("--output,-o", output)->required();
    char *argv[] = {(char*)"required", nullptr};
    int argc = 1;
    XCLI_PARSE(app, argc, argv);
    return 0;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        std::cerr << "Usage: error_demo <duplicate|excludes|required>\n";
        return 1;
    }
    if(std::strcmp(argv[1], "duplicate") == 0)
        return demo_duplicate();
    if(std::strcmp(argv[1], "excludes") == 0)
        return demo_excludes();
    if(std::strcmp(argv[1], "required") == 0)
        return demo_required();
    std::cerr << "unknown: " << argv[1] << '\n';
    return 1;
}
