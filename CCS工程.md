# CCS工程

## 1. 头文件的添加

​	右键Project Explorer中的CCS项目，选择Properties，在Build->Include Options中，Add dir to #include search path中添加头文件的路径。

> ~/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/include
>
> ~/controlSUITE/device_support/F2837xD/v210/F2837xD_common/include
>
> ~/controlSUITE/device_support/F2837xD/v210/F2837xD_headers/include

## 2. 多核DSP中CPU的指定

​	右键Project Explorer中的CCS项目，选择Properties，在Build->Predefine Symbols中，Pre-define NAME中添加CPU名称，如CPU1。 

## 3. RAM与FLASH的选择

​	在CCS工程中放入command文件2837xD_Headers_nonBIOS_cpu1.cmd。

### 3.1 RAM：

​	在CCS工程中放入RAM command文件2837xD_RAM_lnk_cpu1.cmd，并将FLASH command文件注释掉。

### 3.2 FLASH：

​	在CCS工程中放入FLASH command文件2837xD_FLASH_lnk_cpu1.cmd，并将RAM command文件注释掉。

​	右键Project Explorer中的CCS项目，选择Properties，在Build->Predefine Symbols中，Pre-define NAME中添加_FLASH。 

## 4. 仿真器的选择

​	在View中打开Target Configurations，在User Defined文件中添加新的.ccxml文件，指定对应的DSP核心板与仿真器型号。