@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\Generator\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\Generator\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\Generator\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d Generator -s --keyword=_ -p ./po/fr -o Generator.pot %LIST%
