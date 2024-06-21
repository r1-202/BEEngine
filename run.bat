g++ -c source_files/main.cpp -I include -I header_files
move *.o object_files
g++ -o executable/test object_files/*.o -L lib -l opengl32 -l freetype -l glfw3 -l gdi32 ^
-l ws2_32 -l mswsock -l advapi32
cd executable
test
cd ..
pause
cls