// Weather station web application based on
//
// Google Charts  : Javascript web charts          : https://developers.google.com/chart
// Crow           : C++ web microframework         : https://crowcpp.org/ ,  https://github.com/CrowCpp/Crow
// cpp-dotenv     : C++ .env parser                : https://github.com/arnholm/dotenv-cpp (based on https://github.com/laserpants/dotenv-cpp )
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
#include "templates/index.h"

// update page returns data for the next HTML page reload
std::string update_page(const std::string& selected_sensor, int ndays)
{
   auto* wsapi = wsapi_server::singleton();

   std::shared_ptr<ws_data> data = wsapi->query(ndays);
   const std::vector<time_t>&  x = data->tstmp();
   const std::vector<double>&  y = data->sensor(selected_sensor);

   // compute 24h running average data
   int plen = 86400;  // seconds/day
   std::vector<ws_data::ValuePair> avg = data->running_avg(selected_sensor,plen);

   // return data to the page, using mustache
  // auto page = crow::mustache::load("index.html");
   auto page = crow::mustache::compile(index_html);
   crow::mustache::context ctx;

   // Create list of time,value data for the chart plot
   size_t list_length = x.size() > y.size() ? y.size() : x.size();
   std::vector<crow::mustache::context> list;
   for (size_t i = 0; i < list_length; i++)
   {
      crow::mustache::context item;
      item["time"]  = x[i];
      item["value"] = y[i];
      list.emplace_back(item);
   }
   ctx["chart_list"] = std::move(list);

   std::vector<crow::mustache::context> running_average;
   for(size_t i=0; i<avg.size(); i++) {
      crow::mustache::context item;
      item["time"]  = avg[i].first;
      item["value"] = avg[i].second;
      running_average.emplace_back(item);
   }
   ctx["running_average"] = std::move(running_average);

   // Create list of sensor name,description as well as selected
   auto sensor_map = ws_data::sensor_map();
   std::vector<crow::mustache::context> sensor_list;
   for (auto& sensor : sensor_map)
   {
       crow::mustache::context item;
       item["name"] = sensor.first;
       item["desc"] = sensor.second;
       item["selected"] = sensor.first == selected_sensor;
       sensor_list.emplace_back(item);
   }
   ctx["sensor_list"] = std::move(sensor_list);

   ctx["title"] = wsapi->getenv("WSAPI_NAME") + " ("+wsapi->getenv("WSAPI_ELEVATION")+"m)";
   ctx["ndays"] = ndays;
   ctx["yaxis"] = ws_data::sensor_map()[selected_sensor];

   ctx["img_url"]    = wsapi->getenv("WSIMAGE_URL");
   ctx["img_hdr"]    = wsapi->getenv("WSIMAGE_HDR");
   ctx["img_astyle"] = wsapi->getenv("WSIMAGE_ASTYLE");
   ctx["img_istyle"] = wsapi->getenv("WSIMAGE_ISTYLE");

   // render the page using mustache context
   return page.render(ctx);
}

int main(int argc, char **argv)
{
   wsapi_server wsapi;
   crow::SimpleApp app;

   CROW_ROUTE(app, "/")([]()
                        {
                           wsapi_server* api = wsapi_server::singleton();
                           return crow::response(200, "html", update_page(api->default_sensor(),api->default_ndays()));
                        });

   CROW_ROUTE(app, "/submit")
  .methods("GET"_method, "POST"_method)([](const crow::request& req) {

      crow::multipart::message msg(req);

      // decode <form> parameters
      multipart_params form(msg);
      int ndays          = form["ndays"].i();
      std::string sensor = form["sensor"];

      // update page with new graph
      return crow::response(200, "html", update_page(sensor,ndays));
   });

   //set the port, set the app to run on multiple threads, and run the app
   app.port(18080).run();

   return 0;
}


