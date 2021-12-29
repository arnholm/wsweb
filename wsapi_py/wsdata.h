#ifndef WSDATA_H
#define WSDATA_H

#include "wsapi_py/wsapi_py_config.h"
#include <memory>
#include "wsapi/ws_data.h"

class WSAPI_PY_PUBLIC wsdata {
public:
   wsdata(std::shared_ptr<ws_data> data);
   virtual ~wsdata();

   // return length of time series
   size_t size() const { return m_data->size(); }

   const std::vector<time_t>& tstmp() const { return m_data->tstmp(); }  // time stamp, unix time
   const std::vector<double>& itemp() const { return m_data->itemp(); }  // indoor temperature
   const std::vector<double>& ihumi() const { return m_data->ihumi(); }  // indoor humidity
   const std::vector<double>& otemp() const { return m_data->otemp(); }  // outdoor temperature
   const std::vector<double>& ohumi() const { return m_data->ohumi(); }  // outdoor humidity
   const std::vector<double>& opres() const { return m_data->opres(); }  // pressure, adjusted to sea level
   const std::vector<double>& owspd() const { return m_data->owspd(); }  // average wind speed
   const std::vector<double>& owgus() const { return m_data->owgus(); }  // gust wind speed
   const std::vector<double>& owdir() const { return m_data->owdir(); }  // wind direction in degrees
   const std::vector<double>& orain() const { return m_data->orain(); }  // total rain in mm

private:
   std::shared_ptr<ws_data> m_data;
};

#endif // WSDATA_H
