@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\ImageEffects\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\ImageEffects\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\ImageEffects\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d ImageEffects -s --keyword=_ -p ./po/fr -o ImageEffects.pot %LIST%
