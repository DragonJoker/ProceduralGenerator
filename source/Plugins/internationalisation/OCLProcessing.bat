@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\OCLProcessing\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\OCLProcessing\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\OCLProcessing\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d OCLProcessing -s --keyword=_ -p ./po/fr -o OCLProcessing.pot %LIST%
