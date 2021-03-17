#include "send_home.hpp"

#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "nlohmann/json.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
//TODO: add to python prepended tag to strip

std::string getIpcName();

void init_connection()
{
    sock.bind(getIpcName());
}

void send_home_image(void* buffer_data, size_t buffer_size)
{
    std::vector<zmq::message_t> msg_rng;
    // reserving the amount of message object required
    msg_rng.reserve(buffer_size/MAX_SIZE);
    auto bytes_sent_sofar = int{0};
    do
    {
        auto read_size = std::min(size_t(MAX_SIZE), size_t(buffer_size - bytes_sent_sofar));
        // give the socket the buffer with the updated underlying data
        msg_rng.emplace_back(buffer_data + bytes_sent_sofar, read_size);
        bytes_sent_sofar += read_size;
    }while(bytes_sent_sofar != buffer_size);
    
    zmq::send_multipart(sock, msg_rng);
}

void send_home_system_id(int id)
{
    std::string description = std::to_string(id);
    zmq::message_t snd(description);
    sock.send(snd, zmq::send_flags::sndmore);
}

std::string getIpcName()
{
    std::ifstream configuration_f(configuration_file_name, std::ifstream::in);
    if (!configuration_f) {
        std::cout << "is there a configuration file in this directory?" 
                "it should have the same name as the one defined in send_home.hpp as" 
                "\'configuration_file_name\'" << std::endl;
    }
    std::string conf; 
    // get length of file:
    configuration_f.seekg (0, configuration_f.end);
    conf.reserve(configuration_f.tellg());
    configuration_f.seekg (0, configuration_f.beg);
    
    conf.assign((std::istreambuf_iterator<char>(configuration_f)),
            std::istreambuf_iterator<char>());

    return nlohmann::json::parse(conf)["ipc_filename"];
}
