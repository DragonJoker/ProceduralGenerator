@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\BuggedLifeGame\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\BuggedLifeGame\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\BuggedLifeGame\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d BuggedLifeGame -s --keyword=_ -p ./po/fr -o BuggedLifeGame.pot %LIST%
