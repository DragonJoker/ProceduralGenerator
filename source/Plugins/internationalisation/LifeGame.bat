@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\LifeGame\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\LifeGame\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\LifeGame\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d LifeGame -s --keyword=_ -p ./po/fr -o LifeGame.pot %LIST%
