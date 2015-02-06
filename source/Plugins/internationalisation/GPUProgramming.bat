@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\GPUProgramming\*.cpp) do set LIST=!LIST! %%x
xgettext -d GPUProgramming -s --keyword=_ -p ../po -o GPUProgramming.pot %LIST%
