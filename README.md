# xcli

[中文版](./README_CN.md)

Header-only C++ command-line parsing library. Forked from [CLI11](https://github.com/CLIUtils/CLI11) with a gflags-compatible global flag system.

## Quick Start

```cpp
#include <xcli/cli.h>

int main(int argc, char **argv) {
    xcli::App app("my tool");

    std::string name;
    int port = 8080;
    bool verbose = false;

    app.add_option("--name,-n", name, "your name");
    app.add_option("--port,-p", port, "listening port");
    app.add_flag("--verbose,-V", verbose, "enable verbose output");

    XCLI_PARSE(app, argc, argv);

    std::cout << "Hello " << name << ", port=" << port << '\n';
}
```

## Two API Styles

### 1. Local variables (CLI11 style)

Small programs, single file — bind directly to local variables:

```cpp
bool flag = false;
app.add_flag("--flag", flag, "description");
std::string val;
app.add_option("--opt", val, "description");
```

### 2. Global flags (gflags style)

Large projects, flags shared across translation units:

```cpp
// main.cc
XDEFINE_bool(verbose, false, "enable verbose output");
app.add_flag("--verbose,-V", XFLAGS_verbose);

// logger.cc
XDECLARE_string(output);
void log(const std::string &msg) {
    std::ofstream out(XFLAGS_output);
    out << msg;
}
```

`XDEFINE_*` supports: `bool`, `int32`, `uint32`, `int64`, `uint64`, `double`, `string`.

## Examples

See [`examples/cli/`](examples/cli/) for 30+ runnable demos:

| File | Topic |
|------|-------|
| `xflags.cc` | gflags globals + App binding |
| `simple.cc` | basic add_flag / add_option |
| `subcommands.cc` | nested subcommands |
| `validators.cc` | input validation |
| `config_app.cc` | TOML/INI config file |
| `formatter.cc` | custom help output |
| `enum.cc` | enum options |
| `option_groups.cc` | option groups |

Build all examples:

```bash
cmake -B build -DKMCMAKE_BUILD_EXAMPLES=ON
cmake --build build
./build/examples/cli/xflags --verbose --port=9090
```

## Build & Install

```bash
cmake -B build
cmake --build build
cmake --install build --prefix /path/to/install
```

Single-header usage: just copy `xcli/` to your include path and `#include <xcli/cli.h>`.

## Test

```bash
ctest --test-dir build
```

## License

Apache 2.0 — see [LICENSE](LICENSE).
