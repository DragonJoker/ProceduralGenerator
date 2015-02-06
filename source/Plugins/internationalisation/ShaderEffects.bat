@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\ShaderEffects\*.cpp) do set LIST=!LIST! %%x
xgettext -d ShaderEffects -s --keyword=_ -p ../po -o ShaderEffects.pot %LIST%
