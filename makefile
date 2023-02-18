program:
	g++ program.cpp game.cpp sprite_renderer.cpp game_object.cpp game_level.cpp include/texture.cpp include/shader.cpp include/resource_manager.cpp /usr/include/glad/glad.c -o game.elf -DLOCAL -std=c++11 -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor;
	chmod +x game.elf;
	./game.elf;
	rm game.elf;
