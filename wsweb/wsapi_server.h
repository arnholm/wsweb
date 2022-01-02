#ifndef wsapi_server_H
#define wsapi_server_H

#include <memory>
#include "wsapi/ws_db.h"
#include "wsapi/ws_data.h"

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


/*
   wsapi_server manages the server side of a weather station web application.
   It is to be considered a singleton object

   - it loads and manages .env variables
   - it connects to the weather station database
   - it manages data queries and returns data
*/


class wsapi_server {
public:
   wsapi_server(const std::string& env_name = "wsapi.env");
   virtual ~wsapi_server();

   static wsapi_server* singleton();

   // get environment variable
   std::string getenv(const std::string& key);

   // get data for ndays before present
   std::shared_ptr<ws_data>  query(int ndays, double min_tdiff=-1.0);

   // list of available sensors
   std::vector<std::string> sensors() const;
   std::vector<std::string> sensors_text() const;

   // return currently selected sensor
   std::string selected_sensor() const { return m_selected_sensor; }

   // return currently selected number of days
   int selected_ndays() const { return m_selected_ndays; }

   // set selected number of days, i.e. for next query
   void set_selected_ndays(int ndays) { m_selected_ndays = ndays; }

   // set selected sensor, i.e. for next query
   void set_selected_sensor(const std::string& selected_sensor) { m_selected_sensor = selected_sensor; }

private:
   std::string              m_env_name; // name of .env file read

   std::shared_ptr<ws_db>   m_db;       // database file
   std::shared_ptr<ws_data> m_data;     // data containing data corresponding to m_ndays
   int                      m_ndays;    // number of days last read from database
   static wsapi_server*     m_self;

private:
   std::string              m_selected_sensor;  // user selection of sensor
   int                      m_selected_ndays;   // user selection of number of days
};

#endif // wsapi_server_H
