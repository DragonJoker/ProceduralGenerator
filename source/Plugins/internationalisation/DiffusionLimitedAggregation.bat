@echo off
setlocal enabledelayedexpansion enableextensions
set LIST=
for %%x in (..\source\DiffusionLimitedAggregation\*.cpp) do set LIST=!LIST! %%x
xgettext -d DiffusionLimitedAggregation -s --keyword=_ -p ../po -o DiffusionLimitedAggregation.pot %LIST%
