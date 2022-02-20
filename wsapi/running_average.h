#ifndef running_average_H
#define RUNNING_AVERAGE_H

#include <ctime>
#include <queue>
#include <map>
using namespace std;
#include "wsapi_config.h"

class WSAPI_PUBLIC running_average {
public:
   typedef pair<time_t,double> ValuePair;
   typedef queue<ValuePair>    ValueQueue;

   running_average(int plen);
   virtual ~running_average();

   // just like a list, push it to the "newest" position in the queue
   void push_back(const ValuePair& value);

   // return the current average value
   double value() const;

   // return the current time for which the value is valid
   time_t time_value() const;

private:
   int        m_plen;   // max length of time period
   ValueQueue m_queue;
   double     m_sum;    // sum of values in queue
};

#endif // RUNNING_AVERAGE_H
