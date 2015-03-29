@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\IteratedFunctionSystem\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\IteratedFunctionSystem\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\IteratedFunctionSystem\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d IteratedFunctionSystem -s --keyword=_ -p ./po/fr -o IteratedFunctionSystem.pot %LIST%
