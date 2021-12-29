#ifndef WS_DATA_H
#define WS_DATA_H

#include "sqlWeatherStation.h"
#include "wsapi_config.h"
#include <map>
#include <vector>

// time series representation of weather station data
class WSAPI_PUBLIC ws_data {
public:
   ws_data();
   virtual ~ws_data();

   // reserve memory for time series of length sz
   void reserve(size_t sz);

   // push one sample to each time series
   void push_back(const sqlWeatherStation::sample& s);

   // return length of time series
   size_t size() const { return m_tstmp.size(); }

   const std::vector<time_t>& tstmp() const { return m_tstmp; }  // time stamp, unix time
   const std::vector<double>& itemp() const { return m_itemp; }  // indoor temperature
   const std::vector<double>& ihumi() const { return m_ihumi; }  // indoor humidity
   const std::vector<double>& otemp() const { return m_otemp; }  // outdoor temperature
   const std::vector<double>& ohumi() const { return m_ohumi; }  // outdoor humidity
   const std::vector<double>& opres() const { return m_opres; }  // pressure, adjusted to sea level
   const std::vector<double>& owspd() const { return m_owspd; }  // average wind speed
   const std::vector<double>& owgus() const { return m_owgus; }  // gust wind speed
   const std::vector<double>& owdir() const { return m_owdir; }  // wind direction in degrees
   const std::vector<double>& orain() const { return m_orain; }  // total rain in mm

private:
   std::vector<time_t> m_tstmp; // time stamp
   std::vector<double> m_itemp; // indoor temperature
   std::vector<double> m_ihumi; // indoor humidity
   std::vector<double> m_otemp; // outdoor temperature
   std::vector<double> m_ohumi; // outdoor humidity
   std::vector<double> m_opres; // pressure
   std::vector<double> m_owspd; // average wind speed
   std::vector<double> m_owgus; // gust wind speed
   std::vector<double> m_owdir; // wind direction in degrees
   std::vector<double> m_orain; // total rain in mm
};

#endif // WS_DATA_H
