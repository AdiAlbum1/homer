#ifndef SEND_HOME__H
#define SEND_HOME__H

#include <zmq.hpp>

/***********API stuff*****************/

constexpr auto configuration_file_name = "configuration.json";

static zmq::context_t ctx;
static zmq::socket_t sock(ctx, zmq::socket_type::push);

void init_connection();

// TODO:the message object memcopies the data into itself.
//      if this is time consuming another solution is needed.
void send_home_image(void* buffer_data, size_t buffer_size);
void send_home_system_id(int id);


/**********non-API stuff(still usefull)**************/

constexpr auto MEGABYTE = 1024L * 1024L;
constexpr auto KILOBYTE = 1024L;

size_t constexpr operator""Mb(long double const x)
{ return MEGABYTE*x; }

size_t constexpr operator""Kb(long double const x)
{ return KILOBYTE*x; }

constexpr auto MAX_SIZE = 0.5Mb;

#endif //SEND_HOME__H