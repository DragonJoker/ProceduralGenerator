@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\ImageEffects\*.cpp) do set LIST=!LIST! %%x
xgettext -d ImageEffects -s --keyword=_ -p ../po -o ImageEffects.pot %LIST%
