@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\PerlinNoise\*.cpp) do set LIST=!LIST! %%x
xgettext -d PerlinNoise -s --keyword=_ -p ../po -o PerlinNoise.pot %LIST%
