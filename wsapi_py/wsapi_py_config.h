#ifndef WSAPI_PY_CONFIG_H_INCLUDED
#define WSAPI_PY_CONFIG_H_INCLUDED

#ifdef _MSC_VER

   /// MSVC compiler

   #ifdef WSAPI_PY_IMPLEMENTATION
      #define WSAPI_PY_PUBLIC __declspec(dllexport)
      #define WSAPI_PY_EXTERN
   #else
      #define WSAPI_PY_PUBLIC __declspec(dllimport)
      #define WSAPI_PY_EXTERN extern
   #endif
   #define WSAPI_PY_PRIVATE

#elif __GNUC__

   /// gnu gcc

   #define WSAPI_PY_PUBLIC  __attribute__ ((visibility("default")))
   #define WSAPI_PY_PRIVATE __attribute__ ((visibility("hidden")))
   #define WSAPI_PY_EXTERN
	
#else

  #error "Unknown compiler"
	
#endif


#endif // WSAPI_PY_CONFIG_H_INCLUDED

