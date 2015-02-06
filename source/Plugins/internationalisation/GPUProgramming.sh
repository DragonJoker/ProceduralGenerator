# example sh script to generate catalog file
# it searches for strings marked with _() in all .cpp files in directory ./src
# the generated file is called 'app_name.pot' and is generated in ./po
CPP_FILE_LIST=`find ../source/GPUProgramming -name '*.cpp' -print`
xgettext -d GPUProgramming -s --keyword=_ -p ../po -o GPUProgramming.pot $CPP_FILE_LIST
