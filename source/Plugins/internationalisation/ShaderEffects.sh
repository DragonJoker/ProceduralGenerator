# example sh script to generate catalog file
# it searches for strings marked with _() in all .cpp files in directory ./src
# the generated file is called 'app_name.pot' and is generated in ./po
CPP_FILE_LIST=`find ../ShaderEffects/Src -name '*.cpp;*.h;*.inl' -print`
xgettext -d ShaderEffects -s --keyword=_ -p ./po/fr -o ShaderEffects.pot $CPP_FILE_LIST
