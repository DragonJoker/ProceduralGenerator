# example sh script to generate catalog file
# it searches for strings marked with _() in all .cpp files in directory ./src
# the generated file is called 'app_name.pot' and is generated in ./po
CPP_FILE_LIST=`find ../ColoredLifeGame/Src -name '*.cpp;*.h;*.inl' -print`
xgettext -d ColoredLifeGame -s --keyword=_ -p ./po/fr -o ColoredLifeGame.pot $CPP_FILE_LIST
