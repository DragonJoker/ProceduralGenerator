@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\PerlinNoise\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\PerlinNoise\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\PerlinNoise\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d PerlinNoise -s --keyword=_ -p ./po/fr -o PerlinNoise.pot %LIST%
