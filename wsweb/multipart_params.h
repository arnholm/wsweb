#ifndef MULTIPART_PARAMS_H
#define MULTIPART_PARAMS_H
#include <crow.h>
#include <map>

/*
** Author: Carsten Arnholm, January 2022
** This code was written for my weather station project
**
** This code follows the sqlite3 license model, i.e.:
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
*/

/*
ref Crow: https://github.com/CrowCpp/Crow
multipart_params decodes parameter values from a crow::multipart::message

example usage with an HTML <form>:
   {
      ...
      crow::multipart::message msg(req);
      multipart_params form(msg);

      int ndays          = form["ndays"].i();  // int parameter
      std::string sensor = form["sensor"];     // string parameter
      ...
   }
*/

class multipart_params {
public:

   using params_map = std::map<std::string,std::string>;
   using iterator = params_map::iterator;

   multipart_params(const crow::multipart::message& msg)
   {
     for(auto& part : msg.parts) {
        auto& headers = part.headers;
        for(auto& h : headers ) {
           auto& params = h.params;
           for(auto& p : params ) {
              if(p.first == "name") {
                m_params[p.second] = part.body;
              }
           }
        }
     }
   }

   virtual ~multipart_params() {}

   bool has(const std::string& key) { return (m_params.find(key) != m_params.end()); }
   iterator find(const std::string& key) { return m_params.find(key); }
   iterator begin() { return m_params.begin(); }
   iterator end() { return m_params.end(); }

   // class to help deal with type conversions on return
   class param {
   public:
      param(iterator i) : m_i(i) {}
      std::string name() { return m_i->first; }
      operator std::string() { return m_i->second; } // return as string
      int    i() { return std::stoi(m_i->second); }  // return as integer
      double d() { return std::stod(m_i->second); }  // return as double
      iterator m_i;
   };

   // look up a key and return it as param object
   param operator[](const std::string& key) { return param(m_params.find(key)); }

private:
   params_map m_params;  // <name,value>
};

#endif // MULTIPART_PARAMS_H
