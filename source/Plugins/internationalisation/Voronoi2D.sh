# example sh script to generate catalog file
# it searches for strings marked with _() in all .cpp files in directory ./src
# the generated file is called 'app_name.pot' and is generated in ./po
CPP_FILE_LIST=`find ../source/Voronoi2D -name '*.cpp' -print`
xgettext -d Voronoi2D -s --keyword=_ -p ../po -o Voronoi2D.pot $CPP_FILE_LIST
