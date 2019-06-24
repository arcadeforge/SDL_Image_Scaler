gcc -o scale_img -lSDL_ttf -lSDL_gfx -lSDL_image -lstdc++  -L/tmp/sdl/lib -Wl,-rpath,/tmp/sdl/lib -lSDL scale_img.cpp
