cmake_minimum_required(VERSION 3.0)
project(Pong)

set(PONG_SRCS 
	${CMAKE_SOURCE_DIR}/src/main.c
	)

add_executable(Pong ${PONG_SRCS})

link_directories(/usr/local/lib)
target_link_libraries(Pong GL glfw3
	# glfw3 library dependencies
	glfw3 rt Xrandr Xinerama Xi Xcursor m dl Xrender drm Xdamage X11-xcb 
		xcb-glx xcb-dri2 xcb-dri3 xcb-present xcb-sync xshmfence Xxf86vm 
		Xfixes Xext X11 pthread xcb Xau Xdmcp
	)
target_include_directories(Pong PUBLIC /usr/local/include)

