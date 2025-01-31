## Clone the project

Clone the LVGL Framebuffer Demo project and its related sub modules.

```
git clone https://github.com/lvgl/lv_port_linux.git
cd lv_port_linux/
git submodule update --init --recursive
```

## cmake

```
cmake -B build -S .
make -C build -j
```
