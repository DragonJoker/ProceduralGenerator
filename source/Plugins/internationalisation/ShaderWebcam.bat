@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\ShaderWebcam\*.cpp) do set LIST=!LIST! %%x
xgettext -d ShaderWebcam -s --keyword=_ -p ../po -o ShaderWebcam.pot %LIST%
