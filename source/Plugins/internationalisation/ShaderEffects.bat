@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\ShaderEffects\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\ShaderEffects\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\ShaderEffects\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d ShaderEffects -s --keyword=_ -p ./po/fr -o ShaderEffects.pot %LIST%
