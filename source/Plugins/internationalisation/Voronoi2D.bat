@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\Voronoi2D\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\Voronoi2D\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\Voronoi2D\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d Voronoi2D -s --keyword=_ -p ./po/fr -o Voronoi2D.pot %LIST%
