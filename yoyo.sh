# Maybe one day I'll learn how to Makefile
# 
# yoyo is just a simple compile-run script, `make` doesn't work on my PC...

reset_ansi='\u001b[0m'
red_ansi='\u001b[31m'
yellow_ansi='\u001b[33m'
magenta_ansi='\u001b[35m'
bold_ansi='\e[1m'
green_ansi='\u001b[32m'
underline_ansi='\e[4m'

cpp="g++"
debug_flags="-Og -ggdb"
max_errors="1"
ld_flags="-Iinclude"
cpp_flags="-std=c++17"
target="bin/yocto"
cpp_files="src/*.cpp src/**/*.cpp"

if [[ $1 = "compile" || $1 = "c" ]]; then
    echo -e "\n${underline_ansi}${magenta_ansi}[Compile source]${reset_ansi}\n"
    eval "${cpp} ${debug_flags} ${ld_flags} ${cpp_files} -o ${target} ${cpp_flags}"
elif [[ $1 = "run" || $1 = "r" ]]; then
    echo -e "\n${yellow_ansi}[Run source]${reset_ansi}\n"
    # Cut first argument (this script name)
    eval "./bin/yocto ${@:2}"
elif [[ $1 = "d" || $1 = "debug" ]]; then
    echo -e "\n${bold_ansi}${green_ansi}[Debug:'${target}']${reset_ansi}\n"
    eval "gdb ${target}"
elif [[ $1 = "git_all_docs_changes" ]]; then
    echo -e "\n${bold_ansi}${green_ansi}Git add, commit and push docs${reset_ansi}\n"
    eval "git add ."
    eval "git commit -m \"docs changes\""
    eval "git push -u origin master"
else
echo "Usage:
    'compile' or 'c' => compile source
    'run' or 'r' => run source
    'debug' or 'd' => debug target
    'ctest' => compile tests
    'test' => run tests"
fi