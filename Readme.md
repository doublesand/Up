## Up

利用 Opengl 实现一个 3D 场景，综合所学知识完成一个场景。

## 实验环境

1. 使用 VS2017 在release 和 X86 模式下运行

2. 使用以下库，利用 Cmake 编译成 X86 的 release 模式的。

   - GLFW库：专门针对OpenGL的C语言库，它提供了一些渲染物体所需的最低限度的接口。
   - GLAD库：基于Core的3.3版本、C++语言（在线生成要保证生成加载器）
   - stb_image库：用来读取纹理图片
   - glm数学矩阵库：0.9.9版本（此时默认初始化为全0矩阵）
   - assimp库：3.3.1版本，用来读取obj文件


## 资源

1. 教程：https://learnopengl.com/
2. 天空图：http://www.custommapmakers.org/skyboxes.php
3. OBJ文件：https://clara.io/library，https://free3d.com/3d-models/obj

## 场景实现

1. 多个虚拟物体组成一个虚拟场景，场景可以是室内或者室外场景；场景应包含地面。

2. 给至少两个物体添加纹理贴图

3. 添加光照、阴影效果

4. 用户交互实现视角切换完成对场景的任意角度浏览

5. 通过交互控制物体

   实现物体的变换，允许用户通过键盘或者鼠标实现场景中至少两个物体的控制（移动，旋转，缩放等等）。

