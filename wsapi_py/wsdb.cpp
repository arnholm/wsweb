#include "wsdb.h"
#include "wsapi/ws_db.h"
#include <stdexcept>

wsdb::wsdb()
{}

wsdb::wsdb(const std::string& db_path, bool create_if_missing)
: m_db(std::make_shared<ws_db>(db_path,create_if_missing))
{}

wsdb::~wsdb()
{}

wsdb wsdb::open_database(const std::string& db_path)
{
   return wsdb(db_path,false);
}

void wsdb::close(bool update)
{
   if(m_db.get()) {
      m_db->close(update);
      m_db = nullptr;
   }
}

void  wsdb::set_elevation(double elevation)
{
   if(m_db.get()) {
      m_db->set_elevation(elevation);
   }
}

double wsdb::get_elevation()
{
   if(m_db.get()) {
      return m_db->get_elevation();
   }
   return 0.0;
}

wsdata  wsdb::get_data_days(int ndays, double min_tdiff)
{
   if(!m_db.get()) throw std::runtime_error("No database open");

   return m_db->get_data_days(ndays,min_tdiff);
}
