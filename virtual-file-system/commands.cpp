// $Id: commands.cpp,v 1.16 2016-01-14 16:10:40-08 - - $

#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

void fn_cat (inode_state& state, const wordvec& words){
   if(words.size() >= 2){
        for(size_t i = 1; i < words.size(); i++){
            string word_file = words[i];
            state.print_file(word_file);
        }
   }

   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_cd (inode_state& state, const wordvec& words){

    if (words.size() >=2){
        string director_listing = words.at(1);
        state.change_directory(director_listing);
    }
    else{
        string directory_listing = "";
        state.change_directory(directory_listing);
    }

   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words){

   state.ls_all();
   cout << endl;
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_lsr (inode_state& state, const wordvec& words){
   string directory;
   if(words.size() >= 2){
       directory = words[1];
       state.ls_all_rec_command(directory);
   } else if (words.size() == 1){
       directory = "";
       state.ls_all_rec_command(directory);
   }

   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_make (inode_state& state, const wordvec& words){

   if(words.size() > 2){
   string path_string = words[1];
   string content_string = "";
   for(size_t i = 2; i < words.size(); i++){
       content_string.append(" ");
       content_string.append(words[i]);
   }

   state.write_file(path_string,content_string);

   }

   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_mkdir (inode_state& state, const wordvec& words){

   if (words.size() >=2){
       string new_folder = words.at(1);
       state.new_directory(new_folder);
   }
   else{

   }
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_prompt (inode_state& state, const wordvec& words){
   //state.prompt()
    if (words.size() >=2){
        string prompt = words.at(1);
        state.set_prompt(prompt);
    }
    else{
        string prompt = "";
        state.set_prompt(prompt);
    }
}

void fn_pwd (inode_state& state, const wordvec& words){
   cout << state.pwd_cwd() << endl;
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

