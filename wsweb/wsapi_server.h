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
   std::string default_sensor() const { return m_default_sensor; }

   // return currently selected number of days
   int default_ndays() const { return m_default_ndays; }

private:
   std::string              m_env_name; // name of .env file read

   std::shared_ptr<ws_db>   m_db;       // database file
   static wsapi_server*     m_self;

private:
   std::string              m_default_sensor;
   int                      m_default_ndays;
};

#endif // wsapi_server_H
