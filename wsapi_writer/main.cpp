#include <iostream>
#include <string>
#include <filesystem>

#include <udpsimple/udp_receiver.h>
#include "ws_consumer.h"

int main(int argc, char **argv)
{
   if(argc < 2) {
      std::cout << "Insufficient arguments provided" << std::endl;
      std::cout << "Usage: wsapi_writer  <database_filename>" << std::endl;
      return 1;
   }

   std::string db_path = argv[1];
   if(!std::filesystem::exists(db_path)) {
      std::cout << "database file does not exist: "<<  db_path << std::endl;
      return 1;
   }

   try  {
      // use default udp port
      udp_receiver receiver;
      receiver.receive(std::make_shared<ws_consumer>(db_path));
   }
   catch (std::exception& e)  {
      std::cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}


