function cc() {
    local src=${1}.cpp
    local res=${1}
    g++ $src -o $res -std=c++20 -Wall -Wextra -Wshadow -fsanitize=leak,address,undefined -g -DCHEISSMART
}

