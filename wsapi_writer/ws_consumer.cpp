#include "ws_consumer.h"

#include "wsapi/ws_register_persistent_types.h"


#include "op_lite/op_manager.h"
#include "op_lite/op_new.h"
#include "op_lite/op_class_name.h"
#include "op_lite/op_transaction.h"
#include <iostream>

ws_consumer::ws_consumer(const std::string& db_path)
: m_db_path(db_path)
, m_db(nullptr)
{
   ws_register_persistent_types();
   m_db = op_mgr()->open_database(m_db_path,m_db_path);
}

ws_consumer::~ws_consumer()
{
  m_db = op_mgr()->close_database(m_db_path,false);
}

void ws_consumer::consume(const unsigned char* data, size_t bytes_transferred)
{
   if(bytes_transferred > 0) {
      try {
         auto p = sqlWeatherStation::parse_udp(std::string(data,data+bytes_transferred));
         if(p.second) {
            // push to in-core list in case db-instantiation fails temporarily
            m_samples.push_front(p.first);
         }

         if(m_samples.size() > 0) {

            // we have something to write, create write-transaction
            op_transaction trans(m_db,false);

            while(m_samples.size() > 0) {

               // oldest sample
               auto first_sample = m_samples.begin();

               // Write only if it does not already exists
               list<op_pid> ids;
               m_db->select_ids(ids,op_typeid<sqlWeatherStation>(),sqlWeatherStation::time_clause(first_sample->tstmp));
               if(ids.size() == 0) {

                  // not found, so we write
                  sqlWeatherStation* obj = op_new_db<sqlWeatherStation>(m_db,*first_sample);

                  // format the time, use gmtime always
                  const size_t buflen = 32;
                  char time_buf[buflen];
                  strftime(time_buf,buflen,"%Y%m%d %H:%M:%S UTC",gmtime(&(first_sample->tstmp)));
                  std::cout << "OK: "<<time_buf<< " : "<< std::string(data,data+bytes_transferred) << std::endl;
               }

               // ok, we managed to write it, so remove from in-core list
               m_samples.erase(first_sample);
            }
         }

         // if we get to this point, we should normally have no pending samples
         if(m_samples.size() > 0) std::cout << "Warning: Samples pending=" << m_samples.size() << std::endl;
         return;
      }
      catch(std::exception& ex) {
         std::cout << "Exception: " << ex.what() << ", samples pending=" << m_samples.size() << std::endl;
      }
   }
   std::cout << "ERROR: "<< std::string(data,data+bytes_transferred) << std::endl;
}
