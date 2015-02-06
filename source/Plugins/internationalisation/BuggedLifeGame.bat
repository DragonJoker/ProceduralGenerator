@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\BuggedLifeGame\*.cpp) do set LIST=!LIST! %%x
xgettext -d BuggedLifeGame -s --keyword=_ -p ../po -o BuggedLifeGame.pot %LIST%
