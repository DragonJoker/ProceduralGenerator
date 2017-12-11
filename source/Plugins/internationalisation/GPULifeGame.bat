@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\GPULifeGame\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\GPULifeGame\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\GPULifeGame\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d GPULifeGame -s --keyword=_ -p ./po/fr -o GPULifeGame.pot %LIST%
