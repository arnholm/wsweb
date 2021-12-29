#include "ws_register_persistent_types.h"
#include "op_lite/op_manager.h"
#include "op_lite/op_ptr.h"

#include "sqlWeatherStation.h"

template <typename T>
void ws_register_persistent_type()
{
   if(!op_mgr()->type_factory()->type_installed(op_typeid<T>())) {
      op_mgr()->type_factory()->install(new op_persistent_class<T>());
   }
}

void ws_register_persistent_types()
{
   ws_register_persistent_type<sqlWeatherStation>();
}
