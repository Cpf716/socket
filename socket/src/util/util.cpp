//
//  util.cpp
//  socket
//
//  Created by Corey Ferguson on 9/3/25.
//

#include "util.h"

std::string trim_end(const std::string string) {
    size_t end = string.length();

    while (end > 0 && isspace(string[end - 1]))
        end--;
        
    return string.substr(0, end);
}