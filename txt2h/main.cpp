#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <filesystem>

// compile the context of a text file into a C++ std::string (hex code)
int main (int argc, char **argv)
{
   if(argc < 2) {
      std::cout << "Insufficient arguments provided" << std::endl;
      std::cout << "Usage: txt2h  <filename>" << std::endl;
      return 1;
   }
   std::filesystem::path path_in  = std::filesystem::path(argv[1]);
   if(!std::filesystem::exists(path_in)) {
      std::cout << "Error: File does not exist: " << path_in << std::endl;
      return 1;
   }

   // construct the output file name
   std::filesystem::path path_out = path_in;
   path_out.replace_extension("h");
   if(path_in == path_out) {
      std::cout << "Error: Input and output file names are the same: " << path_in << std::endl;
      return 1;
   }

   std::ifstream in(path_in,std::ios::in);
   if(!in.is_open()) {
      std::cout << "Error: could not open for read: " << path_in << std::endl;
      return 1;
   }
   // get the time stamps of input and output files
   // if output file does not exist, it is marked as older than input file, thus out of date
   auto time_in  = std::filesystem::last_write_time(path_in);
   auto time_out = time_in - std::chrono::seconds{1};
   if(std::filesystem::exists(path_out)) time_out = std::filesystem::last_write_time(path_out);

   if(time_in > time_out) {

      // output file must be updated
      std::ofstream out(path_out);
      if(!out.is_open()) {
         std::cout << "Error: could not open for write: " << path_out << std::endl;
         return 1;
      }

      // read all data from input file
      std::string file_data {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};

      // construct variable name: e.g. filename 'index.html' becomes 'index_html'
      std::string var_name = path_in.stem().string() + '_' + path_in.extension().string().substr(1);

      // write as array of hex data
      size_t counter=0;
      out << "#pragma once " << std::endl;
      out << "#include <string>" << std::endl;
      out << "const std::string " << var_name << " {";
      for(size_t i=0; i<file_data.size(); i++) {

         // write in blocks of 16
         if(counter % 16 == 0) out << std::endl;

         // separator
         if(i==0) out <<"  ";
         else     out <<", ";

         // write as hex
         out << std::hex << "0x"<<(int)file_data[i];
         counter++;
      }
      out << " };";
   }
   return 0;
}

