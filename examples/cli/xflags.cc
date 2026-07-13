#include <xcli/cli.h>
#include <xcli/flags.h>
#include <iostream>

XDEFINE_bool(verbose, false, "enable verbose output");
XDEFINE_string(output, "", "output file path");
XDEFINE_int32(port, 8080, "listening port");

int main(int argc, char **argv) {
    xcli::App app("xflags demo");

    // --verbose, --output, --port 直接绑定到 XFLAGS_xxx 全局变量
    app.add_flag("--verbose,-V", XFLAGS_verbose, "enable verbose output");
    app.add_option("--output,-o", XFLAGS_output, "output file path");
    app.add_option("--port,-p", XFLAGS_port, "listening port");

    auto *run = app.add_subcommand("run", "run server");
    // 子命令上也可以绑定同一个 XFLAGS
    run->add_flag("--verbose,-V", XFLAGS_verbose, "enable verbose output");
    run->add_option("--port,-p", XFLAGS_port, "listening port");

    XCLI_PARSE(app, argc, argv);

    std::cout << "verbose: " << (XFLAGS_verbose ? "true" : "false") << '\n';
    std::cout << "output: '" << XFLAGS_output << "'\n";
    std::cout << "port: " << XFLAGS_port << '\n';
}
