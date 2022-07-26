# 创新创业实践课项目

## 个人信息

姓名：马郁欣

班级：2019级信安班

学号：201901460071

github账户：[Tougherr](https://github.com/Tougherr)



## 项目简介

共完成5个项目，部分算法调用开源代码库[GmSSL](https://github.com/guanzhi/GmSSL)，环境配置依照[方法](https://blog.csdn.net/vincy_yeha/article/details/120572903)

设计编程语言c++、python

## 运行指导
python文件在安装gmssl库后可直接运行

c++文件在安装GmSSL后通过vs编译器导入sln文件加载整个项目

## 完成项目列表

- [x] Implement the naive birthday attack of reduced SM3
- [x] Implement the Rho method of reduced SM3
- [x] Do your best to optimize SM3 implementation (software)
- [x] Implement Merkle Tree following RFC6962
- [x] Implement a PGP scheme with SM2
- [x] Implement SM2 2P sign with real network communication


其余项目未完成


## 项目介绍

### naive birthday attack

对SM3进行生日攻击，寻找n bit碰撞。

对于给定消息m=“birthday attack”，计算其256bit哈希值h=sm3(m)

<img src="https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730165622.png" alt="QQ截图20220730165622" style="zoom: 80%;" />

attack存储寻找的碰撞，每次计算attack的哈希值并比较指定的前n比特。若不相等则attack存储内容+1

运行结果如下

- n=16bit

  ![image](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730163735.png))

- n=24bit

  ![(https://github.com/Tougherr/SDU_project/pic/QQ截图20220730163900.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730163900.png)



### Rho method 

从初始值出发不断进行哈希直到出现一个满足前n比特碰撞的环路

![[image](https://github.com/Tougherr/SDU_project/pic/180798636-473eb919-4214-4648-b23f-630955c4fc2f.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/180798636-473eb919-4214-4648-b23f-630955c4fc2f.png)

该项目实现可调整所得环的大小rholen以及碰撞的比特数n

初始值设置为sm3("Rho method")，在构造出一个具有rholen个哈希值的环路后不断循环更新环上哈希值直至首尾哈希值前n比特相同

样例运行结果如下

![[image](https://github.com/Tougherr/SDU_project/pic/QQ截图20220730184143.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730184143.png)



### optimize SM3

在SM3基础上进行加速，减少循环调用并进行循环展开以及SIMD

如：

![[image](https://github.com/Tougherr/SDU_project/pic/QQ截图20220730194614.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730194614.png)

进行100000次运算加速前后对比如下：

![[image](https://github.com/Tougherr/SDU_project/pic/QQ截图20220730194705.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730194705.png)

速度由2951ms提升到1761ms

### Merkle Tree

基于python 实现Merkle Tree，运行方式如下：

python tree.py -m 【method】 -s 【size】 -c 【string】

>-m：build则进行哈希树构建，check则验证给定内容是否存在
>
>-s：merkle 哈希树规模
>
>-c：要验证的内容

- 对于树的构建：

![[image](https://github.com/Tougherr/SDU_project/pic/QQ截图20220730195509.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730195509.png)

会创建size个随机字符串用于构建，之后调用buildTree函数存储在merkle.tree函数中

- 对于存在验证

  若存在则返回相关的哈希值，否则显示不存在，如下：

  ![[image](https://github.com/Tougherr/SDU_project/pic/QQ截图20220730200031.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730200031.png)



### PGP scheme with SM2

整体可分为加密方（发送方）和解密方（接收方）

- 对于加密方，流程如下

  <img src="https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730200807.png" alt="QQ截图20220730200807" style="zoom:80%;" />

  对M和session key分别使用sm2和sm4进行加密，发送给解密方![[image](https://github.com/Tougherr/SDU_project/pic/QQ截图20220730201157.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730201157.png)

- 对于解密方，流程如下：<img src="https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730200816.png" style="zoom:80%;" />

  首先将session key解密得到会话密钥再用会话密钥解密消息

  ![[image](https://github.com/Tougherr/SDU_project/pic/QQ截图20220730201540.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730201540.png)

运行结果如下：

![[image](https://github.com/Tougherr/SDU_project/pic/QQ截图20220730202338.png)](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220730202338.png)



### SM2 2P sign

![QQ截图20220731133836](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220731133836.png)

基于上图所示实现基于sm2的签名过程，分为client1.py和client2.py两方，首先运行client2.py等待连接的建立，再运行client1.py建立连接后进行三轮交互，ec.py中实现椭圆曲线上的基本运算操作。

运行结果如下：

![QQ截图20220731134411](https://github.com/Tougherr/SDU_project/blob/main/pic/QQ%E6%88%AA%E5%9B%BE20220731134411.png)
