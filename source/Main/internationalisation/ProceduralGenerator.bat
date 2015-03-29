@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\ProceduralGenerator\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\ProceduralGenerator\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\ProceduralGenerator\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d ProceduralGenerator -s --keyword=_ -p ./po/fr -o ProceduralGenerator.pot %LIST%
