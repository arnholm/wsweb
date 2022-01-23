#include <iostream>
#include <string>
#include <filesystem>

#include <udpsimple/udp_receiver.h>
#include "ws_consumer.h"

#include <dotenv.h>

int main(int argc, char **argv)
{
   dotenv::init("wsapi.env");
   std::string db_path = dotenv::getenv("WSAPI_DB");
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


