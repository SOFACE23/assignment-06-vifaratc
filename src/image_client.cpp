//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fstream>

using boost::asio::ip::tcp;

//Changed image size to match recieving buffer stream.
constexpr size_t image_size = 17618;

void save_image(char* data, size_t len)
{
  //Spcifies outputfile name (and extension-name), then writes recieved data to this location.
  std::ofstream outputFile("copyCat.jpg");
  outputFile.write(data,len);
}

int main(int argc, char* argv[])
{
  try
  {
    //Checks if program was executed with correct number of arguments (Perhaps one forgot to state the ip..?:) ) 
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    //Resolver makes sure the ip given as an argument is correctly handled as a list of endpoint objects.
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "daytime");

    //Connects socket object to the created endpoint.
    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    while(true)
    {
      boost::array<char, image_size> buf;
      boost::system::error_code error;

      // read until buffer is full
      boost::asio::read(socket,boost::asio::buffer(buf));
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      //Writes data read from server to a file, using save_image().
      save_image(buf.data(),image_size);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}