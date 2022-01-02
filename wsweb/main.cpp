// Weather station web application based on
//
// Google Charts  : Javascript web charts          : https://developers.google.com/chart
// Crow           : C++ web microframework         : https://crowcpp.org/ ,  https://github.com/CrowCpp/Crow
// cpp-dotenv     : C++ .env parser                : https://github.com/adeharo9/cpp-dotenv
// op_lite        : C++ object oriented database   : https://github.com/arnholm/cpde_syslibs/tree/master/op_lite
//

/*
** Author: Carsten Arnholm, January 2022
** This code was written for my weather station project
**
** This code follows the sqlite3 license model, i.e.:
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
*/

// CROW_MAIN is required in the main application file
#define CROW_MAIN
#include <crow.h>

#include "wsapi_server.h"
#include "multipart_params.h"

// update page returns data for the next HTML page reload
std::string update_page()
{
   auto* wsapi = wsapi_server::singleton();

   std::string selected_sensor = wsapi->selected_sensor();
   int ndays                   = wsapi->selected_ndays();

   std::shared_ptr<ws_data>  data = wsapi->query(ndays);

   std::vector<time_t> x = data->tstmp();
   std::vector<double> y = data->sensor(selected_sensor);

   // return data to the page, using mustache
   auto page = crow::mustache::load("index.html");
   crow::mustache::context ctx;
   ctx["title"]           = wsapi->getenv("WSAPI_NAME") + " ("+wsapi->getenv("WSAPI_ELEVATION")+"m)";
   ctx["sensors"]         = wsapi->sensors();
   ctx["sensors_text"]    = wsapi->sensors_text();
   ctx["selected_sensor"] = selected_sensor;
   ctx["ndays"]           = ndays;
   ctx["xpoints"]         =  x;
   ctx["ypoints"]         =  y;
   ctx["yaxis"]           =  ws_data::sensor_map()[selected_sensor];

   // render the page using mustache context
   return page.render(ctx);
}

int main(int argc, char **argv)
{
   wsapi_server wsapi;
   crow::SimpleApp app;

   CROW_ROUTE(app, "/")([]() { return update_page(); });

   CROW_ROUTE(app, "/submit")
  .methods("GET"_method, "POST"_method)([](const crow::request& req) {

      crow::multipart::message msg(req);

      // decode <form> parameters
      multipart_params form(msg);
      int ndays          = form["ndays"].i();
      std::string sensor = form["sensor"];

      // store the parameter values
      auto* wsapi = wsapi_server::singleton();
      wsapi->set_selected_ndays(ndays);
      wsapi->set_selected_sensor(sensor);

      // update page with new graph
      return update_page();
   });

   //set the port, set the app to run on multiple threads, and run the app
   app.port(18080).multithreaded().run();

   return 0;
}


