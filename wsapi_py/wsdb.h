#ifndef WSDB_H
#define WSDB_H

#include "wsapi_py/wsapi_py_config.h"
#include <memory>
#include "wsdata.h"
class ws_db;

// python wrapper class for weather station database
class WSAPI_PY_PUBLIC wsdb {
public:
   wsdb();
   wsdb(const std::string& db_path, bool create_if_missing);
   virtual ~wsdb();

   // open an existing database. throw exception if it does not exist
   static wsdb open_database(const std::string& db_path);

   // set elevation in meters
   void set_elevation(double elevation);

   // get elevation in meters
   double get_elevation();

   // get data for ndays before present
   wsdata get_data_days(int ndays,double min_tdiff);

   // close this database
   void close(bool update);

private:
   std::shared_ptr<ws_db> m_db;
};

#endif // WSDB_H
