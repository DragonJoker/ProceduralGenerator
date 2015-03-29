@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\GPUProgramming\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\GPUProgramming\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\GPUProgramming\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d GPUProgramming -s --keyword=_ -p ./po/fr -o GPUProgramming.pot %LIST%
