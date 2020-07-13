# Utils

在项目的CMakeLists.txt中使用`add_subdirectory(Utils)`添加本文件夹，作为一个动/静态库链接到项目`target_link_libraries(${ProjectName} utils)`。

1. Logger：一个简单的命令行程序日志出输出工具。
    
    使用前必备：
    - 下载第三方开源库 [fmtlib](https://github.com/fmtlib/fmt) ，建议使用`git submodule https://github.com/fmtlib/fmt.git 3rd-party`，或者从github上直接下载并解压在项目目录下`./3rd-party/`下，因为本代码中默认的fmt路径为`./3rd-party/`。使用CMake管理项目，在 CMakeLists.txt 中添加`add_subdirectory(3rd-party/fmt)`。
 
    使用方法：根据想要输出级别的不同，使用不同的print函数，例如输出一个debug级别的log，`Logger::debug_print("some sentence {} {} {}", param1, param2, param3)`，更多函数内参数的写法可以完全按照fmtlib中的 [README](https://github.com/fmtlib/fmt/blob/master/README.rst) 填写。
