#include "sqlWeatherStation.h"
#include "op_lite/op_manager.h"
#include "op_lite/op_database.h"
#include "op_lite/op_ptr.h"
#include "op_lite/op_class_name.h"
#include "op_lite/op_transaction.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>

#include "ws_data.h"
#include <locale>

inline void tokenize(const std::string& input,
                     const std::string& delimiters,
                     std::vector<std::string>& tokens)
{
   using namespace std;
   string::size_type last_pos = 0;
   string::size_type pos = 0;
   while(true) {
      pos = input.find_first_of(delimiters, last_pos);
      if( pos == string::npos ) {
         if(input.length()-last_pos > 0)tokens.push_back(input.substr(last_pos));
         break;
      }
      else {
         if(pos-last_pos > 0)tokens.push_back(input.substr(last_pos, pos - last_pos));
         last_pos = pos + 1;
      }
   }
}



sqlWeatherStation::sqlWeatherStation(const sample& data)
: m_time("time_t",data.tstmp,sqPK_1)
, m_itemp("itemp",data.itemp)   // we don't use op_construct here, to allow "m_" prefixes for the class member variables
, m_ihumi("ihumi",data.ihumi)
, m_otemp("otemp",data.otemp)
, m_ohumi("ohumi",data.ohumi)
, m_opres("opres",data.opres)
, m_owspd("owspd",data.owspd)
, m_owgus("owgus",data.owgus)
, m_owdir("owdir",data.owdir)
, m_orain("orain",data.orain)
, m_osens("osens",data.osens)
{}

sqlWeatherStation::sqlWeatherStation()
: m_time("time_t",sqPK_1)
, m_itemp("itemp")    // we don't use op_construct here, to allow "m_" prefixes for the class member variables
, m_ihumi("ihumi")
, m_otemp("otemp")
, m_ohumi("ohumi")
, m_opres("opres")
, m_owspd("owspd")
, m_owgus("owgus")
, m_owdir("owdir")
, m_orain("orain")
, m_osens("osens")
{}

sqlWeatherStation::~sqlWeatherStation()
{}

void sqlWeatherStation::op_layout(op_values& pvalues)
{
   op_bind(pvalues,m_time);
   op_bind(pvalues,m_itemp);
   op_bind(pvalues,m_ihumi);
   op_bind(pvalues,m_otemp);
   op_bind(pvalues,m_ohumi);
   op_bind(pvalues,m_opres);
   op_bind(pvalues,m_owspd);
   op_bind(pvalues,m_owgus);
   op_bind(pvalues,m_owdir);
   op_bind(pvalues,m_orain);
   op_bind(pvalues,m_osens);
}

std::pair<sqlWeatherStation::sample,bool> sqlWeatherStation::parse_udp(const std::string& message)
{
   std::pair<sqlWeatherStation::sample,bool> res;
   res.second = false;

   // Set the locale to the POSIX C environment
   std::locale l = locale::global(locale("C"));

   std::vector<std::string> tokens;
   tokenize(message,",",tokens);
   if(tokens.size() == 12 && tokens[0] =="$WSDATA") {

      sqlWeatherStation::sample& s = res.first;
      try {

         size_t i=0;
         s.tstmp = static_cast<time_t>(std::stoul(tokens[++i]));
         s.itemp = std::stod(tokens[++i]);
         s.ihumi = std::stod(tokens[++i]);
         s.otemp = std::stod(tokens[++i]);
         s.ohumi = std::stod(tokens[++i]);
         s.opres = std::stod(tokens[++i]);
         s.owspd = std::stod(tokens[++i]);
         s.owgus = std::stod(tokens[++i]);
         s.owdir = std::stoul(tokens[++i]);
         s.orain = std::stoul(tokens[++i]);
         s.osens = std::stoul(tokens[++i]);
         if(s.osens==1)res.second = true;
      }
      catch(...) {
         res.second = false;
      }
   }

   // reset the locale
   std::locale::global(l);

   return res;
}

double sqlWeatherStation::opres(double elevation) const
{
   // http://en.wikipedia.org/wiki/Barometric_formula
   double otemp_K = 273.15 + m_otemp;
   return m_opres / (exp((-9.80665*0.0289664*elevation)/(8.31432*otemp_K)));
}

double sqlWeatherStation::owdir_deg() const
{
   return m_owdir*WS_WIND_DEG;
}

double sqlWeatherStation::orain() const
{
   return m_orain*WS_RAIN_MM;
}

string sqlWeatherStation::time_clause(time_t t_begin, time_t t_end)
{
   ostringstream qout;
   if(t_begin > 0 || t_end > 0) {
      qout << "WHERE ( ";
      if(t_begin > 0)qout << " ( time_t >= " << t_begin << " ) ";
      if(t_begin > 0 && t_end > 0)qout << " AND ";
      if(t_end   > 0)qout << " ( time_t <= " << t_end << " ) ";
      qout << " )";
   }

   return qout.str();
}

string sqlWeatherStation::time_clause(time_t t)
{
   ostringstream qout;
   if(t > 0) {
      qout << "WHERE ( ";
      qout << " ( time_t == " << t << " ) ";
      qout << " )";
   }
   return qout.str();
}


time_t sqlWeatherStation::time_instance(int day_offset)
{
   time_t now;
   time(&now);

   return now + day_offset*24*60*60;
}

bool sqlWeatherStation::is_valid() const
{
   if(m_osens != 1)return false;
   if(m_itemp <  -50.0 || m_itemp >  +100.0 )return false;
   if(m_otemp < -100.0 || m_otemp >  +100.0 )return false;
   if(m_opres < +500.0 || m_opres > +1500.0 )return false;
 //  if(/*m_ohumi <    1.0 ||*/ m_ohumi >  100.0  )return false;
   return true;
}

sqlWeatherStation::sample sqlWeatherStation::get_sample(double elevation)
{
   sample s;

   s.tstmp = time_utc();
   s.itemp = itemp();
   s.ihumi = ihumi();
   s.otemp = otemp();
   s.ohumi = ohumi();
   s.opres = opres(elevation);
   s.owspd = owspd();
   s.owgus = owgus();
   s.owdir = owdir();
   s.orain = orain();
   s.osens = osens();

   return s;
}

std::shared_ptr<ws_data> sqlWeatherStation::get_data(op_database* db, time_t t_beg, time_t t_end, double elevation, double min_tdiff)
{
   std::shared_ptr<ws_data> data = std::make_shared<ws_data>();

   // start a read only transaction
   bool readonly=true;
   op_transaction trans(readonly);

   time_t now;
   time(&now);
   if(min_tdiff < 0.0) {
      double tdiff = difftime(now,t_beg);
      size_t nsamp = tdiff/(2*60);
      if(nsamp > 1024) nsamp = 1024;
      min_tdiff = (nsamp>0)? tdiff/nsamp : 0.0;
   //   cout << nsamp << ' ' << min_tdiff<< endl;
   }


   // build the time series
   std::list<op_pid> ids;
   db->select_ids(ids,op_typeid<sqlWeatherStation>(),time_clause(t_beg,t_end));
   data->reserve(ids.size());
   time_t prev_t = 0;
   size_t icount=0;
   for(auto& id : ids) {
      op_ptr<sqlWeatherStation> ws(id);
      icount++;
      if(ws->is_valid()) {
         time_t this_t = ws->time_utc();
         if(min_tdiff > 0.0) {
            double tdiff = difftime(this_t, prev_t);
            if(tdiff >= min_tdiff || icount==ids.size()) {
               data->push_back(ws->get_sample(elevation));
               prev_t = this_t;
            }
         }
         else {
            data->push_back(ws->get_sample(elevation));
            prev_t = this_t;
         }
      }
   }

//   cout << " data.size() = " << data->size() << endl;
   return data;
}

std::shared_ptr<ws_data> sqlWeatherStation::get_data_days(op_database* db, int ndays, double elevation, double min_tdiff)
{
   time_t t_beg = time_instance(-ndays);
   time_t t_end = 0;
   return get_data(db,t_beg,t_end,elevation,min_tdiff);
}
