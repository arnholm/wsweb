#ifndef WS_CONSUMER_H
#define WS_CONSUMER_H

#include "udpsimple/udp_consumer.h"
#include <string>
#include <list>
#include "wsapi/sqlWeatherStation.h"

class op_database;

// write incoming weather station UDP messages to local database
class ws_consumer : public udp_consumer {
public:
   ws_consumer(const std::string& db_path);
   virtual ~ws_consumer();

   // virtual method called by udp_receiver
   virtual void consume(const unsigned char* data, size_t bytes_transferred);

private:
   std::string  m_db_path;
   std::list<sqlWeatherStation::sample> m_samples;
};

#endif // WS_CONSUMER_H
