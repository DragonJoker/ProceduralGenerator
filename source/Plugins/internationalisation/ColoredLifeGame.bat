@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\ColoredLifeGame\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\ColoredLifeGame\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\ColoredLifeGame\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d ColoredLifeGame -s --keyword=_ -p ./po/fr -o ColoredLifeGame.pot %LIST%
