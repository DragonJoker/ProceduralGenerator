@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\ColoredLifeGame\*.cpp) do set LIST=!LIST! %%x
xgettext -d ColoredLifeGame -s --keyword=_ -p ../po -o ColoredLifeGame.pot %LIST%
