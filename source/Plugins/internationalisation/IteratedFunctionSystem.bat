@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\IteratedFunctionSystem\*.cpp) do set LIST=!LIST! %%x
xgettext -d IteratedFunctionSystem -s --keyword=_ -p ../po -o IteratedFunctionSystem.pot %LIST%
