# example sh script to generate catalog file
# it searches for strings marked with _() in all .cpp files in directory ./src
# the generated file is called 'app_name.pot' and is generated in ./po
CPP_FILE_LIST=`find ../LifeGame/Src -name '*.cpp;*.h;*.inl' -print`
xgettext -d LifeGame -s --keyword=_ -p ./po/fr -o LifeGame.pot $CPP_FILE_LIST
