
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <string>
using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"
#include <fstream>

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;


#include "util.h"
#include "process_file.h"


void process_file(istream& infile,
                  string& input_file_name,
                  str_str_map& map){
    TRACE('r',"PROCESSING FILE: " <<input_file_name)
    string file_line;
    string value = "";
    string key = "";
    bool has_assign;
    bool is_comment;
    for(size_t count = 1;;++count){
        if(infile.eof()){
            break;
        }
        getline(infile,file_line);
        file_line = trim_string(file_line);

        is_comment = line_is_comment(file_line);
        has_assign = line_has_equals(file_line);
        key = line_get_key(file_line);
        value = line_get_value(file_line);
        key = trim_string(key);
        value = trim_string(value);
        TRACE('r',"INPUT LINE: " << file_line)
        TRACE('r',"LINE KEY  : " << key)
        TRACE('r',"LINE VALUE: " << value)
        TRACE('r',"HAS ASSIGN: " << has_assign)
        TRACE('r',"IS COMMENT: " << is_comment)
        cout << input_file_name <<": "
             <<count<< ": "
            << file_line<<endl;



        //Is line a comment? (# ...)
        //DONE
        if (is_comment){
            TRACE('r',":: IS COMMENT ::");
            continue;
        }
        //Is line a key lookup ( key ...)
        //DONE
        else if((key!="")and(!has_assign)){
            TRACE('r',":: IS LOOKUP ::");
            str_str_map::iterator iter = map.find(key);
            if(iter != map.end()){
                cout << iter->first << " = " << iter->second << endl;
            }else{
                cout <<"key :"<< key << " not found" << endl;
            }


        }
        //Is line a key/value deletion? (key = ...)
        else if ((key!="")and(has_assign)and(value=="")){
            TRACE('r',":: IS DELETION ::");
            str_str_map::iterator iter = map.find(key);
            if( iter != map.end()){
                map.erase(iter);
                cout << key << " deleted ";
            }




        }
        //Is line a key/value assignment (key = value)
        //DONE
        else if((key!="")and(has_assign)and(value!="")){
             TRACE('r',":: IS KEY-VALUE ASSIGNMENT ::");
             str_str_pair pair(key,value);
             map.insert(pair);
             cout << key << " = " << value << endl;


        }
        //Is line a print all statement? (=)
        //DONE
        else if((key=="")and(has_assign)and(value=="")){
             TRACE('r',":: IS PRINT ALL ::");
             str_str_map::iterator iter = map.begin();
             while(iter != map.end()){
                 cout << iter->first << " = " << iter->second << endl;
                 ++iter;
             }

        }

        //Is line a print by value statement? (=value)
        //DONE
        else if((key=="")and(has_assign)and(value!="")){
             TRACE('r',":: IS PRINT BY VALUE ::");
             str_str_map::iterator iter = map.begin();
             while(iter != map.end()){
                 if(value == iter->second)
                 cout << iter->first << " = " << iter->second << endl;
                 ++iter;
             }
        }


    }




}



