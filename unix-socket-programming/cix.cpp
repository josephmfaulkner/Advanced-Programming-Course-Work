// $Id: cix.cpp,v 1.2 2015-05-12 18:59:40-07 - - $

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"


logstream log (cout);
struct cix_exit: public exception {};


unordered_map<string,cix_command> command_map {
   {"exit", CIX_EXIT},
   {"get",  CIX_GET },
   {"help", CIX_HELP},
   {"ls"  , CIX_LS  },
   {"put",  CIX_PUT },
   {"rm"  , CIX_RM  },
};

vector<string> split (const string& line, const string& delimiters){
   vector<string> words;
   size_t end = 0;
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   return words;
}


void cix_get(client_socket& server, string filename){
    log << "called cix_get: " << filename <<endl;
    if (filename.size() >= FILENAME_SIZE){
        log << "rm: " << filename
            << ": filename exeeds "
            << FILENAME_SIZE
            << " characters"<< endl;
    } else {
        cix_header header;
        header.nbytes = 0;
        header.command = CIX_GET;
        log << "sending header " << header << endl;
        strcpy(header.filename, filename.c_str());
        send_packet(server, &header, sizeof header);
        recv_packet(server, &header, sizeof header);
        log << "received header " << header << endl;
        if (header.command != CIX_FILE) {
            log << "sent CIX_GET, server did not return CIX_FILE" << endl;
            log << "server returned " << header << endl;
        }else {
            ofstream file_stream(filename,ofstream::out);
            char file_buffer[header.nbytes + 1];
            recv_packet (server,file_buffer, header.nbytes);
            log << "recieved" << header.nbytes << "bytes" << endl;
            file_buffer[header.nbytes] = '\0';
            file_stream.write(file_buffer,header.nbytes);
            file_stream.close();
        }

    }
}



void cix_help() {
   static vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = CIX_LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != CIX_LSOUT) {
      log << "sent CIX_LS, server did not return CIX_LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

void cix_put(client_socket& server, string filename){
    log << "called cix_put: " << filename<< endl;
    if (filename.size() >= FILENAME_SIZE){
        log << "rm: " << filename
            << ": filename exeeds "
            << FILENAME_SIZE
            << " characters"<< endl;
    } else {

        cix_header header;
        header.command = CIX_PUT;
        strcpy(header.filename,filename.c_str());
        ifstream filestream(filename);
        stringstream string_stream;
        if(! filestream.good()){
            log << "put: " << header.filename
                << "no such file exists " << endl;
        } else {
            string_stream << filestream.rdbuf();
            string file_data = string_stream.str();

            //memset(header.filename,0, FILENAME_SIZE);
            header.nbytes = file_data.size();

            log << "sending header " << header << endl;
            send_packet(server, &header, sizeof header);
            send_packet(server,file_data.c_str(),file_data.size());
            recv_packet (server, &header, sizeof header);
            if(header.command != CIX_ACK ){
                log << "sent: " << filename
                    << " : the server didn't accept the file" << endl;
                log << "server returned: " << header << endl;
            }

            log << "sent " << file_data.size() << " bytes" << endl;

            filestream.close();
        }
    }

}



void cix_rm(client_socket& server, string filename){
    log << "called cix_rm: " << filename << endl;
    if (filename == "cixd"){
        log << "Don't you delete the server!" << endl;
    }
    else if (filename == "cix"){
        log << "And don't you delete the client either!" << endl;
    }
    else if (filename.size() >= FILENAME_SIZE){
        log << "rm: " << filename
            << ": filename exeeds "
            << FILENAME_SIZE
            << " characters"<< endl;
    }
    else {
        cix_header header;
        header.command = CIX_RM;
        strcpy(header.filename,filename.c_str());
        log << "sending header " << header << endl;
        send_packet(server, &header, sizeof header);
        recv_packet(server, &header, sizeof header);
        if (header.command != CIX_ACK){
            log << "sent rm: "
                << filename
                << ": cannot delete file "
                << endl;
            log << "server returned "
                << header
                << endl;
        } else{
            log << "sent rm: " << filename
                << ": file deleted " << endl;
            log << "server returned "
                << header << endl;
        }
    }
}




void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         string command = "";
         string filename = "";
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         log << "command " << line << endl;
         vector<string> line_args = split(line, " \t");
         if (line_args.size() < 1)
            command = "";
         else
            command = line_args.at(0);
         if (line_args.size() < 2)
             filename = "";
         else
             filename = line_args.at(1);
         const auto& itor = command_map.find (command);
         cix_command cmd = itor == command_map.end()
                         ? CIX_ERROR : itor->second;
         switch (cmd) {
            case CIX_EXIT:
               throw cix_exit();
               break;


            case CIX_GET:
               if (filename == ""){
                   log << "get: usage: get filename" << endl;
                   break;
               }
               else{
                    cix_get(server,filename);
                    break;
               }

            case CIX_HELP:
               cix_help();
               break;


            case CIX_LS:
               cix_ls (server);
               break;


            case CIX_PUT:
                if (filename == ""){
                    log << "put: usage: put filename" << endl;
                    break;
                }
                else{
                    cix_put(server,filename);
                    break;
                }


            case CIX_RM:
                if (filename == ""){
                    log << "rm: usage: rm filename" << endl;
                    break;
                }
                else {
                    cix_rm(server,filename);
                    break;
                }


            default:
                log << line << ": invalid command" << endl;
                break;

         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}

