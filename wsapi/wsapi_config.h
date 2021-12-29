#ifndef WSAPI_CONFIG_H_INCLUDED
#define WSAPI_CONFIG_H_INCLUDED

#ifdef _MSC_VER

   /// MSVC compiler

   #ifdef WSAPI_IMPLEMENTATION
      #define WSAPI_PUBLIC __declspec(dllexport)
      #define WSAPI_EXTERN
   #else
      #define WSAPI_PUBLIC __declspec(dllimport)
      #define WSAPI_EXTERN extern
   #endif
   #define WSAPI_PRIVATE

#elif __GNUC__

   /// gnu gcc

   #define WSAPI_PUBLIC  __attribute__ ((visibility("default")))
   #define WSAPI_PRIVATE __attribute__ ((visibility("hidden")))
   #define WSAPI_EXTERN
	
#else

  #error "Unknown compiler"
	
#endif


#endif // WSAPI_CONFIG_H_INCLUDED

