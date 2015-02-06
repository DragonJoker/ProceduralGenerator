@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\LifeGame\*.cpp) do set LIST=!LIST! %%x
xgettext -d LifeGame -s --keyword=_ -p ../po -o LifeGame.pot %LIST%
