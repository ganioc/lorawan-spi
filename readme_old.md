# GW1302S

原先的项目的**readme.md**现改名为**SX1302-LoRa-Gateway-project**

## 项目说明

本项目是针对GW1302模块，对原semtech的代码进行了修改，主要是删除了I2C和温度部分，因为此模块上没有相关的硬件

当然开发者也可以使用官方的代码，基本上也是去掉I2C和文档部分的代码即可使用

## 文档说明

- doc文件夹下有整理的FAQ即其答案，可能会有助于开发者您碰到问题时解决问题
- doc下也有原理图
- 若准备使用一键部署脚本，请自己阅读**doc/安装说明.md**文档


## 关于GPIO7与SPI0.1冲突

-- 转接板从1301系列就开始做了,而1301的时候，官方参考复位IO是GPIO7。

随着树莓派软件和硬件的更新，GPIO7被用于SPI0的第二个片选了，即SPI0.1。

如果用早期的软件系统没有冲突。
 

解决办法：

1. 软件办法：本质上Debian系统的IO就是具有很高的复用性的，所以只要修改IO口复用就可以了。

	禁止GPIO7作为片选功能，在pi的boot/config.txt中，最后一行增加
		dtoverlay=spi0-1cs
	可以在pi系统中用sudo管理权限编辑改文件，或SD卡内编辑。

2. 采用新转接板，芮捷电子已出采用官方GPIO23作为复位脚的新转接板。