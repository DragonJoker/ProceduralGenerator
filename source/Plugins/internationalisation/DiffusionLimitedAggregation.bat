@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\DiffusionLimitedAggregation\Src\*.cpp) do set LIST=!LIST! %%x
for %%x in (..\DiffusionLimitedAggregation\Src\*.h) do set LIST=!LIST! %%x
for %%x in (..\DiffusionLimitedAggregation\Src\*.inl) do set LIST=!LIST! %%x
xgettext -d DiffusionLimitedAggregation -s --keyword=_ -p ./po/fr -o DiffusionLimitedAggregation.pot %LIST%
