# 环境
windows </br>
vscode </br>
python 3.7以上</br>
cmake 4.1.1</br>
mingw gcc 15.2.0</br>
[最新！MinGW-w64的下载与安装（超详细！！！） - 知乎](https://zhuanlan.zhihu.com/p/26143367916)
[VS Code终端配置：Shell路径与环境变量-CSDN博客](https://blog.csdn.net/gitblog_01084/article/details/151847047)

windows vscode中powershell与cmd不同环境变量也不同
先用
```bash
$env:PATH  // cmd 是 echo %PATH%
```
查看PATH变量，如没有可添加
```bash
$env:Path += ";D:\software\cmake-4.1.1\bin"
```


# 项目结构
```
my_project/
├── CMakeLists.txt       # CMake 配置文件
├── include/             # 头文件目录
│   └── mylib.h
├── src/                 # 源文件目录
│   ├── main.cpp
│   └── mylib.cpp
├── build/               # 构建目录（可选）
└──	build.py             # 构建脚本
```

# build.py:

- `--clean`: 清理构建目录
- `--config`: 选择构建配置(Debug/Release)
- `--log-dir`: 指定日志文件目录

1. 基本构建（Release模式）：
```bash
python build.py
```

2. Debug模式构建：
```bash
python build.py --config Debug
```

3. 清理构建目录：
```bash
python build.py --clean
```

4. 指定日志目录：
```bash
python build.py --log-dir /path/to/logs
```

---
>手动流程：build 中 

> cd .\build 

> cmake .. -G "MinGW Makefiles" 

> mingw32-make

这是一个用于自动化构建C++项目的Python脚本。让我详细解释其功能和实现原理：

### 主要功能
1. 项目构建自动化：使用CMake和MinGW Makefiles来构建C++项目
2. 日志记录：记录构建过程中的所有输出
3. 构建配置：支持Debug和Release两种构建模式
4. 清理功能：可以清理之前的构建文件

### 核心组件解析

#### 1. run_command函数
```python
def run_command(command, cwd=None, log_file=None):
```
这是脚本的核心功能函数，用于执行系统命令并记录输出。特点：
- 实时显示命令输出到终端
- 同时将输出保存到日志文件
- 记录命令执行的时间戳
- 处理命令执行失败的情况

实现原理：
- 使用`subprocess.Popen`启动子进程执行命令
- 通过`stdout`管道实时读取输出
- 使用行缓冲(`bufsize=1`)确保实时性
- 将标准错误重定向到标准输出(`stderr=subprocess.STDOUT`)

#### 2. main函数
处理命令行参数和构建流程：

**参数解析：**
- `--clean`: 清理构建目录
- `--config`: 选择构建配置(Debug/Release)
- `--log-dir`: 指定日志文件目录

**构建流程：**
1. 设置项目目录和构建目录
2. 处理清理选项
3. 创建必要的目录
4. 设置日志文件
5. 执行CMake配置命令
6. 执行mingw32-make构建命令

### 使用示例

1. 基本构建（Release模式）：
```bash
python build.py
```

2. Debug模式构建：
```bash
python build.py --config Debug
```

3. 清理构建目录：
```bash
python build.py --clean
```

4. 指定日志目录：
```bash
python build.py --log-dir /path/to/logs
```

### 注意事项

1. 环境要求：
   - 需要安装CMake
   - 需要安装MinGW
   - 需要Python环境

2. 构建系统：
   - 使用MinGW Makefiles作为生成器
   - 适用于Windows平台的C++项目

3. 日志管理：
   - 日志文件名包含时间戳和构建配置
   - 自动创建日志目录（如果不存在）
   - 完整记录构建过程，便于问题排查

4. 错误处理：
   - 命令执行失败时会立即终止构建
   - 提供清晰的错误信息
   - 确保日志文件正确关闭

这个脚本特别适合需要频繁构建的C++项目，可以简化构建流程，同时提供完整的构建日志记录。通过命令行参数可以灵活控制构建过程，适合集成到自动化构建系统中。