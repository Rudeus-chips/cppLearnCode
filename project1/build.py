import os
import subprocess
import sys
import argparse
from pathlib import Path
from datetime import datetime
import shutil

def run_command(command, cwd=None, log_file=None):
    """运行命令，将输出同时显示在终端上并保存到日志文件"""
    try:
        print(f"执行命令: {' '.join(command)}")
        
        # 打开日志文件（如果指定了）
        log_fp = None
        if log_file:
            log_fp = open(log_file, 'a', encoding='utf-8')
            # 写入时间戳和命令信息
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            log_fp.write(f"\n\n===== {timestamp} =====\n")
            log_fp.write(f"命令: {' '.join(command)}\n")
            log_fp.write("输出:\n")
        
        # 启动进程
        process = subprocess.Popen(
            command,
            cwd=cwd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,  # 行缓冲
            universal_newlines=True
        )
        
        # 实时处理输出
        for line in process.stdout:
            # 在终端上显示
            print(line, end='')
            # 写入日志文件
            if log_fp:
                log_fp.write(line)
        
        # 等待进程完成
        process.wait()
        
        # 关闭日志文件
        if log_fp:
            log_fp.close()
        
        if process.returncode != 0:
            print(f"命令执行失败，返回码: {process.returncode}")
            return False
        return True
    except Exception as e:
        print(f"执行命令时出错: {e}")
        if log_fp:
            log_fp.close()
        return False

def main():
    # 解析命令行参数
    parser = argparse.ArgumentParser(description="项目构建脚本")
    parser.add_argument("--clean", action="store_true", help="清理构建目录")
    parser.add_argument("--config", default="Release", choices=["Debug", "Release"], help="构建配置")
    parser.add_argument("--log-dir", help="指定日志文件目录")
    args = parser.parse_args()
    
    # 获取项目根目录
    project_root = Path(__file__).parent.absolute()
    build_dir = project_root / "output"
    
    # 清理选项
    if args.clean and build_dir.exists():
        print(f"清理构建目录: {build_dir}")
        shutil.rmtree(build_dir)
        return
    elif args.clean:
        print("构建目录不存在，无需清理")
        return
    
    # 创建build目录（如果不存在）
    if not build_dir.exists():
        print(f"创建构建目录: {build_dir}")
        build_dir.mkdir()
    
    # 确定日志目录
    log_dir = Path(args.log_dir) if args.log_dir else build_dir
    if not log_dir.exists():
        log_dir.mkdir(parents=True)
    
    # 创建日志文件
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    log_file = log_dir / f"build_{timestamp}_{args.config}.log"
    print(f"日志将保存到: {log_file}")
    
    # 步骤1: 进入build目录
    print(f"切换到构建目录: {build_dir}")
    os.chdir(build_dir)
    
    # 步骤2: 运行CMake配置
    cmake_command = ["cmake", "..", "-G", "MinGW Makefiles", f"-DCMAKE_BUILD_TYPE={args.config}"]
    if not run_command(cmake_command, log_file=log_file):
        print("CMake配置失败")
        sys.exit(1)
    
    # 步骤3: 运行mingw32-make进行构建
    make_command = ["mingw32-make"]
    if not run_command(make_command, log_file=log_file):
        print("构建失败")
        sys.exit(1)
    
    print(f"构建成功完成! 配置: {args.config}")
    print(f"日志已保存到: {log_file}")
    

if __name__ == "__main__":
    main()
