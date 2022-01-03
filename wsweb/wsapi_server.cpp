#include "wsapi_server.h"
#include <stdexcept>

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

#include <dotenv.h>

#include "wsapi/ws_data.h"

wsapi_server* wsapi_server::m_self = nullptr;

wsapi_server* wsapi_server::singleton()
{
   return m_self;
}

wsapi_server::wsapi_server(const std::string& env_name)
: m_env_name(env_name)
, m_ndays(7)
, m_selected_ndays(7)
, m_selected_sensor("otemp")
{
   if(m_self) throw std::runtime_error("Canot instantiate more than one wsapi_server object");
   dotenv::init(m_env_name.c_str());

   std::string db_path = dotenv::getenv("WSAPI_DB");
   double elevation    = std::stod(dotenv::getenv("WSAPI_ELEVATION"));

   bool create_if_missing = false;
   m_db = std::make_shared<ws_db>(db_path,create_if_missing);
   m_db->set_elevation(elevation);
   m_data = m_db->get_data_days(m_ndays);

   m_self = this;
}

wsapi_server::~wsapi_server()
{
   m_self = nullptr;
}

std::string wsapi_server::getenv(const std::string& key)
{
   return dotenv::getenv(key.c_str());
}

std::shared_ptr<ws_data> wsapi_server::query(int ndays, double min_tdiff)
{
   if(ndays != m_ndays) {
      m_data = m_db->get_data_days(ndays,min_tdiff);
   }
   m_ndays = ndays;
   return m_data;
}

std::vector<std::string> wsapi_server::sensors() const
{
   std::vector<std::string> slist;
   for(auto p : ws_data::sensor_map()) {
      slist.push_back(p.first);
   }
   return slist;
}

std::vector<std::string> wsapi_server::sensors_text() const
{
   std::vector<std::string> slist;
   for(auto p : ws_data::sensor_map()) {
      slist.push_back(p.second);
   }
   return slist;
}
