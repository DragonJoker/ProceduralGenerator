@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\ShaderWebcam\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\ShaderWebcam\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\ShaderWebcam\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d ShaderWebcam -s --keyword=_ -p ./po/fr -o ShaderWebcam.pot %LIST%
