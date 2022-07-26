# Simulator project for LVGL embedded GUI Library

The [LVGL](https://github.com/lvgl/lvgl) is written mainly for microcontrollers and embedded systems however you can run the library **on your PC** as well without any embedded hardware. The code written on PC can be simply copied when your are using an embedded system.

Using a PC simulator instead of an embedded hardware has several advantages:
* **Costs $0** because you you don't have to buy or design PCB
* **Fast** because you don't have to design and manufacture PCB
* **Collaborative** because any number of developers can work in the same environment
* **Developer friendly** because much easier and faster to debug on PC

## Requirements
This project is configured for VSCode and only tested on Linux, although this may work on OSx or WSL. It requires a working version of GCC, GDB and make in your path.

To allow debugging inside VSCode you will also require a GDB [extension](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) or other suitable debugger.

* **SDL** a low level driver library to use graphics, handle mouse, keyboard etc.

## Usage

### Get the PC project

Clone the PC project and the related sub modules:

```
git clone --recursive https://github.com/Theo-Tsang/lvgl_v3s_prj
```

### Install SDL
You can download SDL from https://www.libsdl.org/

On on Linux you can install it via terminal:
```
sudo apt-get update && sudo apt-get install -y build-essential libsdl2-dev
```

### Setup
To allow custom UI code an `lv_conf.h` file placed at `ui/simulator/inc` will automatically override this projects lv_conf.h file. By default code under `ui` is ignored so you can reuse this repository for multiple projects. You will need to place a call from `main.c` to your UI's entry function.

To build and debug, press F5. You should now have your UI displayed in a new window and can access all the debug features of VSCode through GDB.

To allow temporary modification between simulator and device code, a SIMULATOR=1 define is added globally.


## New
lv_examples/src/lv_demo_smarthome -> UI of [SmartHome project](https://github.com/Theo-s-Open-Source-Project/SmartHome) .

you need to modified main.c
```
int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();            // 初始化lvgl    

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init();           // 这里单片机就是硬件初始化，模拟器中是模拟器初始化(修改配置)

  // lv_demo_widgets();    // 这个函数就是演示的demo(修改示例)
  lv_demo_smarthome(0.5);

  while (1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_task_handler();  // lvgl的任务处理，主要处理事件响应
    usleep(5 * 1000);   // 休眠10ms
  }

  return 0;
}
```

演示视频：
[![Watch the video]()](https://www.bilibili.com/video/BV1rd4y1Q7zW#reply121345889232)
