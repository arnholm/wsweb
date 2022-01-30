#ifndef SQLWEATHERSTATION_H
#define SQLWEATHERSTATION_H

#include "op_lite/op_object.h"
class op_database;
#include <string>
#include <memory>
#include  <time.h>
using namespace std;
class ws_data;
#include "wsapi_config.h"

// http://code.google.com/p/weatherpoller/
#define WS_RAIN_MM 0.3
#define WS_WIND_DEG 22.5

// sqlWeatherStation privides access to a single measurement instance
class WSAPI_PUBLIC sqlWeatherStation : public op_object {
public:
   /// in-core representation of a weather station sample
   struct WSAPI_PUBLIC sample
   {
      sample() : tstmp(0),itemp(0),ihumi(0),otemp(0),ohumi(0),opres(0),owspd(0),owgus(0),owdir(0),orain(0),osens(0) {}
      time_t tstmp;     // unix time (time_t)
      double itemp;     // indoor temperature};
      double ihumi;     // indoor humidity
      double otemp;     // outdoor temperature#endif // WS_DATA_H
      double ohumi;     // outdoor humidity
      double opres;     // pressure
      double owspd;     // average wind speed
      double owgus;     // gust wind speed
      size_t owdir;     // wind direction [1-15], 0 for invalid direction
      size_t orain;     // total rain counter. Multiply by WS_RAIN_MM to get total rain in mm
      size_t osens;     // 0 when outside sensor contact lost, otherwise 1
   };

   sqlWeatherStation();
   sqlWeatherStation(const sample& data);

   virtual ~sqlWeatherStation();

   // parse quasi nmea-style message into sample object, bool flag indicates if parsing was successful
   static std::pair<sample,bool> parse_udp(const std::string& message);

   // convenience function to declare this as persistent class
   // called at program startup
   static void declare_persistent();

   /// get persistence layout of this type
   /*! /param pvalues container of persistent member variables */
   virtual void op_layout(op_values& pvalues);

   // returns true if the sensor values are within expected ranges
   bool is_valid() const;

   time_t time_utc() const { return (time_t)m_time; }
   double itemp() const    { return m_itemp; }
   double ihumi() const    { return m_ihumi; }

   double otemp() const    { return m_otemp; }
   double ohumi() const    { return (m_ohumi <= 100.0)? m_ohumi : 0.0; }  // 255% should be 0%
   double opres(double elevation=0.0) const;  // pressure adjusted to sea level
   double owspd() const    { return m_owspd; }
   double owgus() const    { return m_owgus; }
   size_t owdir() const    { return m_owdir; }
   double owdir_deg() const;
   double orain() const;
   size_t osens() const    { return m_osens; }

   // get sample, adjusted for elevetion [m]
   sample get_sample(double elevation = 0);

   // generate a time clause string for the time span
   // an open interval can be specified with t_begin=0 or t_end=0
   static string time_clause(time_t t_begin, time_t t_end);

   // generate a time clause time instance
   static string time_clause(time_t t);

   // compute a time relative to now. (times in the past require negative day_offset)
   static time_t time_instance(int day_offset);

   // get data for given time span
   // an open interval can be specified with t_begin=0 or t_end=0
   // elevation is meters above sea level. Used for adjusting pressure to sea level
   static std::shared_ptr<ws_data> get_data(op_database* db, time_t t_begin, time_t t_end, double elevation, double min_tdiff=-1.0);

   // get data for a ndays before present
   static std::shared_ptr<ws_data> get_data_days(op_database* db, int ndays, double elevation, double min_tdiff=-1.0);

private:
   op_int    m_time;   // unix time (time_t)

   // indoor sensors
   op_double m_itemp;  // indoor temperature [C]
   op_double m_ihumi;  // indoor humidity [%]

   // outdoor sensors
   op_double m_otemp;  // outdoor temperature [C]
   op_double m_ohumi;  // outdoor humidity [%]
   op_double m_opres;  // pressure [mbar]
   op_double m_owspd;  // wind speed [m/s]
   op_double m_owgus;  // wind gust [m/s]
   op_int    m_owdir;  // wind direction [deg]
   op_int    m_orain;  // rain accumulated [mm]
   op_int    m_osens;  // outdoor sensors available [1/0]
};

#endif // SQLWEATHERSTATION_H
