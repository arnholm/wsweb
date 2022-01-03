#include "ws_db.h"
#include <stdexcept>
#include "ws_register_persistent_types.h"

#include "op_lite/op_manager.h"
#include "op_lite/op_database.h"
#include "op_lite/op_transaction.h"
#include "op_lite/op_ptr.h"
#include "op_lite/op_new.h"

#include "sqlWeatherStation.h"

ws_db::ws_db()
{
   throw std::runtime_error("ws_db cannot be constructed directly");
}

ws_db::ws_db(const std::string& db_path, bool create_if_missing)
: m_db_path(db_path)
, m_elevation(0)
{
   // make sure the persistent types are registered in op_lite
   ws_register_persistent_types();

   op_database* db = op_mgr()->open_database(db_logical(),db_path);
   if(!db) {
      if(create_if_missing) {
         db = op_mgr()->create_database(db_logical(),db_path);
      }
      else throw std::runtime_error("wsapi: Error opening " + db_path);
   }
}

void ws_db::close(bool update)
{
   op_mgr()->close_database(db_logical(),update);
}

ws_db::~ws_db()
{
   close(false);
}

std::shared_ptr<ws_data> ws_db::get_data_days(int ndays, double min_tdiff)
{
   if(op_database* db = op_mgr()->get_database(db_logical())) {
      return sqlWeatherStation::get_data_days(db,ndays,m_elevation,min_tdiff);
   }
   return std::make_shared<ws_data>();
}
