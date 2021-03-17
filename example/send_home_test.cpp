#include "send_home.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <unistd.h>

int main()
{
    init_connection();
    std::cout << "Connected" << std::endl;
    
    std::array<char, 1.0Mb> large;
    std::ifstream file("./data to send/download.jpeg", std::ios::binary);
    file.read((char*)large.data(), large.size());
    const auto file_size = file.gcount();

    send_home_system_id(1);
    while(true)
    {
        send_home_image(large.data(), file_size);
        send_home_image(large.data(), file_size);
        sleep(10);
    }
    return 0;
}