#include "running_average.h"
#include <limits>
running_average::running_average(int plen)
: m_plen(plen)
, m_sum(0.0)
{}

running_average::~running_average()
{}


void running_average::push_back(const ValuePair& value)
{
   m_queue.push(value);
   time_t end_time = value.first;
   m_sum += value.second;

   while((end_time-m_queue.front().first) > m_plen) {
      m_sum -= m_queue.front().second;
      m_queue.pop();
   }
}


double running_average::value() const
{
   double average = std::numeric_limits<double>::quiet_NaN();
   size_t qsize = m_queue.size();
   if(qsize > 0) {
      double qlen = double(m_queue.back().first) - double(m_queue.front().first);
      if(qlen/double(m_plen) > 0.9) {
         average = m_sum/double(qsize);
      }
   }
   return average;
}

time_t running_average::time_value() const
{
   time_t av_time = 0;
   size_t qsize = m_queue.size();
   if(qsize > 0) {
      int dsec = difftime(m_queue.back().first,m_queue.front().first);
      av_time = m_queue.front().first + dsec/2;
   }
   return av_time;
}
