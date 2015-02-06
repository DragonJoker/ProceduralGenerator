@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\ProceduralGenerator\*.cpp) do set LIST=!LIST! %%x
xgettext -d ProceduralGenerator -s --keyword=_ -p ../po -o ProceduralGenerator.pot %LIST%
