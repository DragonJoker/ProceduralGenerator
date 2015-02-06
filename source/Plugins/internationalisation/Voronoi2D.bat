@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\Voronoi2D\*.cpp) do set LIST=!LIST! %%x
xgettext -d Voronoi2D -s --k=_ -p ../po -o Voronoi2D.pot %LIST%
