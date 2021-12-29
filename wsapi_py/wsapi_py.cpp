#include "wsapi_py.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "wsdb.h"
#include "wsdata.h"

PYBIND11_MODULE(wsapi_py, m) {

   m.doc() = "Weather Station Python plugin"; // optional module docstring

   py::class_<wsdb>(m,"wsdb")
     .def(py::init(&wsdb::open_database), py::arg("db_path"), "Open existing database")
     .def("set_elevation",&wsdb::set_elevation, py::arg("elevation")=0.0, "Set sensor elevation in [m] above sea level")
     .def("get_elevation",&wsdb::get_elevation, "Get sensor elevation in [m] above sea level")
     .def("get_data_days",&wsdb::get_data_days, py::arg("ndays"), py::arg("min_tdiff")=-1.0, "Get data for ndays in the past")
     .def("close",&wsdb::close, py::arg("update")=false, "Close database")
   ;

   py::class_<wsdata>(m,"wsdata")
     .def("tstmp",&wsdata::tstmp, "Get time stamps as unix times")
     .def("itemp",&wsdata::itemp, "Get indoor temperature")
     .def("ihumi",&wsdata::ihumi, "Get indoor humidity")
     .def("otemp",&wsdata::otemp, "Get outdoor temperature")
     .def("ohumi",&wsdata::ohumi, "Get outdoor humidity")
     .def("opres",&wsdata::opres, "Get pressure, adjusted to sea level")
     .def("owspd",&wsdata::owspd, "Get average wind speed")
     .def("owgus",&wsdata::owgus, "Get gust wind speed")
     .def("owdir",&wsdata::owdir, "Get wind direction in degrees")
     .def("orain",&wsdata::orain, "Get total rain in mm")
   ;



}
