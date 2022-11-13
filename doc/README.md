# Luat_EC618_SDK

#### 介绍
CSDK for EC618, 适用于所有基于EC618的合宙模组, 例如Air780E系列, Air600E系列

#### 软件架构

![image-20221110180352693](C:\Users\WL\AppData\Roaming\Typora\typora-user-images\image-20221110180352693.png)

#### 目录说明

* PLAT 打包好的sdk, 含协议栈, 部分源码, 头文件, 打包工具
* project 项目文件, 每个不同的项目代表不同的demo或者turnkey方案
* xmake.lua 主编译脚本, 一般不需要修改

#### 编译说明

1. 安装xmake, 选取加入PATH, 建议安装到C:\Program Files下，其他目录可能会出问题；
   xmake下载地址：https://xmake.io/#/guide/installation
   windows下可以直接访问：https://github.com/xmake-io/xmake/releases ,下载系统对应的xmake-dev-win32.exe或者xmake-dev-win64.exe即可
2. 如果编译example的话在本代码库的根目录执行`build.bat example`进行编译
4. 生成的binpkg位于`out`目录, 日志数据库文件位于`PLAT`目录

#### 加入入口函数
* 加入头文件common_api.h
* 类似于`void main(void)`，但是可以任意名字，不能有入口参数和返回参数，同时允许多个入口
* 通过INIT_HW_EXPORT INIT_DRV_EXPORT INIT_TASK_EXPORT这3个宏修饰后，系统初始化的时候会调用function，不同的宏涉及到调用顺序不同
* 大优先级上`INIT_HW_EXPORT` > `INIT_DRV_EXPORT` > `INIT_TASK_EXPORT`
* 这3个宏有统一用法`INIT_HW_EXPORT(function, level)`，其中function就是入口函数，level是小优先级，填"0","1","2"...(引号要写的)，引号里数字越小，优先级越高。

* `INIT_HW_EXPORT`一般用于硬件初始化，GPIO之类的，可以没有
* `INIT_DRV_EXPORT`一般用于外设驱动初始化，初始化外部器件之类的，打开电源，提供时钟之类的，可以没有
* `INIT_TASK_EXPORT`一般用于初始化任务，用户代码基本上都是跑在任务里，原则上必须有

#### 加入自己的项目
1. project新建目录，目录名称和项目名称一致，目录下新建xmake.lua，内容仿照着example写，核心是TARGET_NAME必须和项目名称一致
2. 代码路径当然不是限制的，在SDK的任何目录，甚至电脑上的其他地方，前提是你会改project里xmake.lua内的源码路径
3. 根目录执行build.bat 你的项目名称