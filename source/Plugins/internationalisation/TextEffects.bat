@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\TextEffects\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\TextEffects\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\TextEffects\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d TextEffects -s --keyword=_ -p ./po/fr -o TextEffects.pot %LIST%
