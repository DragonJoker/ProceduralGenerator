@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\TextEffects\*.cpp) do set LIST=!LIST! %%x
xgettext -d TextEffects -s --keyword=_ -p ../po -o TextEffects.pot %LIST%
