For a test run *ON WINDOWS* download the files, run "compile_all.bat" then run "run.bat". *Needs gcc*.



This project depends on freetype. A copy of freetype source files along with an automatic compilation batch file is included.
If you already have freetype built simply delete the "lib/libfreetype.a" file and add your copy to the "lib" folder.
Otherwise run "compile_freetype.bat" from the terminal. This compiles freetype and puts the statically linked library in the lib file. *Needs MINGW*.

If further problems arise you may need to recompile glfw.