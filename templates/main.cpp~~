// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"
#include <map>
#include <fstream>
#include "process_file.h"

int arg_count;
using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;


void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            arg_count++ ;
            traceflags::setflags (optarg);
            break;
         default:
            arg_count++ ;
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}




int main (int argc, char** argv) {
   arg_count = 0;
   int exit_is = 0;
   arg_count = 0;
   string execname = argv[0];
   sys_info::set_execname (execname);
   string input_file_name{"-"};
   scan_options (argc, argv);
   TRACE('r',"STARTED PROGRAM");
   str_str_map str_map;


   if ((argc - arg_count) < 2){
       TRACE('r',"ARRAY_COUNT: "<< arg_count)
       TRACE('r',"ARRAY_COUNT_DIFF: "<< (argc - arg_count))
       process_file(cin,input_file_name,str_map);

   }

   else{

       for(int count = arg_count; count < argc; count++){
           TRACE('r',"ARRAY_COUNT: "<< count)
           TRACE('r',"ARRAY_COUNT_DIFF: "<< (argc - arg_count))
           input_file_name = argv[count];
           TRACE('r',"INPUT FILE: " << input_file_name);
           if(input_file_name == "-"){
              process_file(cin,input_file_name,str_map);

           }else{
               ifstream new_in(input_file_name);
               if( ! new_in.fail()){
                   process_file(new_in,input_file_name,str_map);

               } else {
                   exit_is ++;
                   cerr << "File not found: "
                        << input_file_name << endl;
                   continue;
               }

           }


       }
   }

   str_str_map::iterator itor = str_map.begin();
   str_map.erase (itor);

   return exit_is;
}

