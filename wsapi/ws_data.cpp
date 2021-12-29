#include "ws_data.h"

ws_data::ws_data()
{}

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

