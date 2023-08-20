@echo off

set compiler=%VULKAN_SDK%\Bin\glslc.exe
%compiler% shader.vert -o vert.spv
%compiler% shader.frag -o frag.spv
pause