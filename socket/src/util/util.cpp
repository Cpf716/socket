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

std::string uuid() {
    std::ostringstream              oss;
    std::random_device              rd;
    std::mt19937_64                 gen(rd());
    std::uniform_int_distribution<> uid;
    
    for (size_t i = 0; i < 8; i++)
        oss << std::hex << uid(gen) % 16;
    
    oss << "-";
    
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 4; j++)
            oss << std::hex << uid(gen) % 16;
        
        oss << "-";
    }
    
    for (size_t i = 0; i < 12; i++)
        oss << std::hex << uid(gen) % 16;
    
    return oss.str();
}
