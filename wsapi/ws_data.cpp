#include "ws_data.h"

ws_data::ws_data()
{
   m_sensors["itemp"] = &ws_data::itemp;
   m_sensors["ihumi"] = &ws_data::ihumi;
   m_sensors["otemp"] = &ws_data::otemp;
   m_sensors["ohumi"] = &ws_data::ohumi;
   m_sensors["opres"] = &ws_data::opres;
   m_sensors["owspd"] = &ws_data::owspd;
   m_sensors["owgus"] = &ws_data::owgus;
   m_sensors["owdir"] = &ws_data::owdir;
   m_sensors["orain"] = &ws_data::orain;
}

ws_data::~ws_data()
{}

void ws_data::reserve(size_t sz)
{
   m_tstmp.reserve(sz);
   m_itemp.reserve(sz);
   m_ihumi.reserve(sz);
   m_otemp.reserve(sz);
   m_ohumi.reserve(sz);
   m_opres.reserve(sz);
   m_owspd.reserve(sz);
   m_owgus.reserve(sz);
   m_owdir.reserve(sz);
   m_orain.reserve(sz);
}

void ws_data::push_back(const sqlWeatherStation::sample& s)
{
   if(s.osens > 0) {
      m_tstmp.push_back(s.tstmp);
      m_itemp.push_back(s.itemp);
      m_ihumi.push_back(s.ihumi);
      m_otemp.push_back(s.otemp);
      m_ohumi.push_back(s.ohumi);
      m_opres.push_back(s.opres);
      m_owspd.push_back(s.owspd);
      m_owgus.push_back(s.owgus);
      m_owdir.push_back(s.owdir*WS_WIND_DEG);
      m_orain.push_back(s.orain*WS_RAIN_MM);
   }
}

const std::vector<double>& ws_data::sensor(const std::string& name)
{
   static std::vector<double> dummy;

   auto it = m_sensors.find(name);
   if(it == m_sensors.end()) return dummy;

   // call the relevant member function
   sensor_name sens = it->second;
   return (this->*sens)();
}

std::map<std::string,std::string> ws_data::sensor_map()
{
   return
   {
      {"itemp","indoor temperature [C]" }
     ,{"otemp","outdoor temperature [C]" }
     ,{"ihumi","indoor humidity [%]" }
     ,{"ohumi","outdoor humidity [%]" }
     ,{"opres","pressure [hPa]" }
     ,{"owspd","wind speed [m s]" }
     ,{"owgus","wind gust [m s]" }
     ,{"owdir","wind direction [deg]" }
     ,{"orain","accumulated rain [mm]" }
   };
}
