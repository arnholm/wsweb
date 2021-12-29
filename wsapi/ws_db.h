#ifndef ws_db_H
#define ws_db_H

#include "wsapi_config.h"
#include <string>
#include "ws_data.h"

class WSAPI_PUBLIC ws_db {
public:
   ws_db();
   ws_db(const std::string& db_path, bool create_if_missing=false);
   virtual ~ws_db();

   // set elevation in meters
   void set_elevation(double elevation) { m_elevation = elevation; }

   // get elevation in meters
   double get_elevation() { return m_elevation; }

   // get data for ndays before present
   std::shared_ptr<ws_data> get_data_days(int ndays, double min_tdiff=-1.0);

   // close the database
   void close(bool update = false);

protected:
   std::string db_logical() const { return m_db_path; }

private:
   std::string m_db_path;
   double      m_elevation;
};

#endif // ws_db_H
