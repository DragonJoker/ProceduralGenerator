@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\Generator\*.cpp) do set LIST=!LIST! %%x
xgettext -d Generator -s --keyword=_ -p ../po -o Generator.pot %LIST%
