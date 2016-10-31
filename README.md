# Linear-Texture-Coordinate-Interpolation-in-Rasterization

This is the source project of paper [Linear Texture Coordinate Interpolation in Rasterization](http://www.jcad.cn/jcadcms/document/attach_manager!download.action?id=4028e4e44bc55348014c2be463d81403) in Chinese.

You can also find the information of this paper in the `publications` page [here](https://hanhonglei.github.io/publications/)

## Howt to use

1. 使用“打开文件”打开一张tga格式的纹理（比如“棋盘.tga”），也可以直接将tga图片拖拽上去。

2. 点击“R”按钮可以再OpenGL绘制和自主栅格化之间切换。

3. 点击“C”可以调整三角形的几何信息；以及采取哪种栅格化方式（两种：文章提出的KIM，以及王老师提出的WANG）。

4. 使用方向键以及换页键可以调整视角。

5. 每种栅格化方式的时间在窗口中给出，单位毫秒。

6. 如果Raster.exe运行不正确，请首先安装vcredist_x86.exe。

7. 大部分算法在Raster.cpp中，并有注释。

8. 增加了调整纹理坐标精度的选项，可以调整保留到纹理坐标小数点后几位。

9. 程序退出后，会在当前文件夹下生成“OutPut.txt”文件，里面保存了各种栅格化算法的统计数据。