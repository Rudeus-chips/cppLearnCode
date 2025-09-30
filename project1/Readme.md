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

### 使用第三方库pipreqs（推荐）
> 只包含本项目用到的库
```bash
pip install pipreqs
```

```bash
pipreqs ./ --encoding=utf8  --force
```

## 一键安装需要的库

```bash
pip install -r requirements.txt
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

