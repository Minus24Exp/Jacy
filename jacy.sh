# Maybe one day I'll learn how to Makefile
# 
# yoyo is just a bunch of useful scripts for myself

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

cpp_flags="-std=c++17 -m64"
cpp_files="src/*.cpp src/**/*.cpp"

target="./bin/target"

if [[ $1 = "compile" || $1 = "c" ]]; then
    echo -e "\n${underline_ansi}${magenta_ansi}[Compile source]${reset_ansi}\n"
    eval "${cpp} ${debug_flags} ${ld_flags} ${cpp_files} -o ${target} ${cpp_flags}"
elif [[ $1 = "run" || $1 = "r" ]]; then
    echo -e "\n${yellow_ansi}[Run source]${reset_ansi}\n"
    # Cut first argument (`yoyo.sh`)
    eval "./${target} ${@:2}"
elif [[ $1 = "d" || $1 = "debug" ]]; then
    echo -e "\n${bold_ansi}${green_ansi}[Debug:'${target}']${reset_ansi}\n"
    eval "gdb ${target}"
elif [[ $1 = "gitall" ]]; then
    if [[ $2 == "" ]]; then
        echo -e "${red_ansi}Please, specify commit message${reset_ansi}"
        exit
    fi

    echo -e "${bold_ansi}${magenta_ansi}"

    echo -e "git add .\n"
    eval "git add ."

    echo -e "${bold_ansi}${yellow_ansi}"

    echo -e "git commit -m \"${@:2}\"\n"
    eval "git commit -m \"${@:2}\""

    echo -e "${bold_ansi}${green_ansi}"

    echo -e "git push -u origin master\n"
    eval "git push -u origin master"

    echo -e "${reset_ansi}"
else
echo "Usage:
    'compile' or 'c' => compile source
    'run' or 'r' => run source
    'debug' or 'd' => debug target
    'gitall' => git add ., git commit -m *argument*, git push -u origin master"
fi