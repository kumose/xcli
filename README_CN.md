# xcli

[English](./README.md)

一个仅头文件的 C++ 命令行解析库。基于 [CLI11](https://github.com/CLIUtils/CLI11)，增加了 gflags 兼容的全局标志系统。

## 快速开始

```cpp
#include <xcli/cli.h>

int main(int argc, char **argv) {
    xcli::App app("my tool");

    std::string name;
    int port = 8080;
    bool verbose = false;

    app.add_option("--name,-n", name, "你的名字");
    app.add_option("--port,-p", port, "监听端口");
    app.add_flag("--verbose,-V", verbose, "开启详细输出");

    XCLI_PARSE(app, argc, argv);

    std::cout << "Hello " << name << ", port=" << port << '\n';
}
```

## 两种 API 风格

### 1. 局部变量（CLI11 风格）

小项目、单文件 —— 直接绑定到局部变量：

```cpp
bool flag = false;
app.add_flag("--flag", flag, "描述");
std::string val;
app.add_option("--opt", val, "描述");
```

### 2. 全局标志（gflags 风格）

大项目、跨文件共享 flag：

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

`XDEFINE_*` 支持：`bool`、`int32`、`uint32`、`int64`、`uint64`、`double`、`string`。

## 示例

参考 [`examples/cli/`](examples/cli/) 目录下的 30+ 个可运行示例：

| 文件 | 主题 |
|------|------|
| `xflags.cc` | gflags 全局标志 + App 绑定 |
| `simple.cc` | 基础 add_flag / add_option |
| `subcommands.cc` | 嵌套子命令 |
| `validators.cc` | 输入校验 |
| `config_app.cc` | TOML/INI 配置文件 |
| `formatter.cc` | 自定义帮助输出 |
| `enum.cc` | 枚举选项 |
| `option_groups.cc` | 选项组 |

编译全部示例：

```bash
cmake -B build -DKMCMAKE_BUILD_EXAMPLES=ON
cmake --build build
./build/examples/cli/xflags --verbose --port=9090
```

## 编译与安装

```bash
cmake -B build
cmake --build build
cmake --install build --prefix /path/to/install
```

直接使用头文件：将 `xcli/` 目录拷贝到项目的 include 路径，`#include <xcli/cli.h>` 即可。

## 测试

```bash
ctest --test-dir build
```

## 许可证

Apache 2.0 —— 参见 [LICENSE](LICENSE)。
