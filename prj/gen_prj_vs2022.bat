@ECHO off
SET script_dir=%~dp0
SET tool_dir=%script_dir%\..\tools\
SET premake=%tool_dir%\premake\premake5.exe

call %premake% vs2022