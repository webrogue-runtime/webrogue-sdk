all: build_glfw_vulkan build_cpu_rendering build_glfw_glxgears_gles build_raylib build_stk build_empty build_sdl_dev build_gtk

CURRENT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

# apps
build_empty:
	$(MAKE) -C empty -f docker.Makefile

build_glfw_glxgears_gles:
	$(MAKE) -C glfw_glxgears_gles -f docker.Makefile

build_glfw_vulkan:
	$(MAKE) -C glfw_vulkan -f docker.Makefile

build_cpu_rendering:
	$(MAKE) -C cpu_rendering -f docker.Makefile

build_raylib:
	$(MAKE) -C raylib -f docker.Makefile

build_stk:
	$(MAKE) -C stk -f docker.Makefile

build_sdl_dev:
	$(MAKE) -C sdl_dev -f docker.Makefile

build_gtk:
	$(MAKE) -C gtk -f docker.Makefile

clean:
	$(MAKE) -C glfw_glxgears_gles clean -f docker.Makefile
	$(MAKE) -C glfw_vulkan clean -f docker.Makefile
	$(MAKE) -C build_cpu_rendering clean -f docker.Makefile
	$(MAKE) -C raylib clean -f docker.Makefile
	$(MAKE) -C gtk clean -f docker.Makefile
	rm -f **/*.wrapp
