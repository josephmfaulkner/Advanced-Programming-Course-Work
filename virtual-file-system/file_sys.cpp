// $Id: file_sys.cpp,v 1.5 2016-01-14 16:16:52-08 - - $

#include <iostream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

#include "debug.h"
#include "file_sys.h"

int inode::next_inode_nr {1};

void inode_state::set_prompt(string& new_prompt){
    this->prompt_.clear();
    this->prompt_.append(new_prompt).append(" ");
}

inode_ptr inode_state::process_directory(string& directory_string){
    inode_ptr result = this->cwd;
    if(directory_string.front()=='/'){
        result = this->root;
    }
    wordvec directories = split(directory_string,"/");

    for(const auto& p: directories){
        string search = p;
        result = result->contents->search_entry(search);
    }

    return result;

}

struct file_type_hash {
   size_t operator() (file_type type) const {
      return static_cast<size_t> (type);
   }
};

ostream& operator<< (ostream& out, file_type type) {
   static unordered_map<file_type,string,file_type_hash> hash {
      {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
      {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
   };
   return out << hash[type];
}

inode_state::inode_state() {
    inode_ptr new_pointer = make_shared<inode>(file_type::DIRECTORY_TYPE);
    string curr_dir = ".";
    string parent_dir = "..";
    new_pointer->contents->add_entry(curr_dir,new_pointer);
    new_pointer->contents->add_entry(parent_dir,new_pointer);
    root = new_pointer;
    cwd = root;
    cwd_name = "/";
    DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");

}



void inode_state::new_directory(const string& filename){
    wordvec directories = split(filename,"/");
    if(directories.size() == 1){
    cwd->contents->mkdir(filename);
    } else{
        string file_name = directories.back();
        directories.pop_back();
        string new_filepath = "";
        for(auto st:directories){
           string dir = st;
           if(new_filepath.size() != 0){
                new_filepath.append("/");
           }
           new_filepath.append(dir);
        }
        this->process_directory(new_filepath)->contents->mkdir(file_name);
    }
}


void inode_state::write_file(string& filename,string& contents){
    wordvec directories = split(filename,"/");
    string file_name = directories.back();
    directories.pop_back();
    string new_filepath = "";
    for(auto st:directories){
        string dir = st;
        if(new_filepath.size() != 0){
            new_filepath.append("/");
        }
           new_filepath.append(dir);
     }

    wordvec new_contents = split(contents," ");
    this->process_directory(new_filepath)->contents->mkfile(file_name, new_contents);

}

void inode_state::print_file(string& filename){
    this->process_directory(filename)->contents->out();
}

void inode_state::change_directory(string& directory_line){
    string result_directoy_list = this->cwd_name;

    if(directory_line==""){
        this->cwd = this->root;
        result_directoy_list.clear();
        result_directoy_list.append("/");
    }
    else{
        wordvec directories = split(directory_line,"/");
        this->cwd = this->process_directory(directory_line);
        if(directory_line.front() == '/'){
            result_directoy_list.clear();
        }
        for(const auto& p: directories){
            string search = p;


            if(search == ""){

            }
            else if(search == "."){

            }
            else if(search == ".."){
                while(result_directoy_list.back() != '/' &&
                      result_directoy_list.size() > 0)
                {
                      result_directoy_list.pop_back();
                }
                if((result_directoy_list.back() == '/') && (result_directoy_list.size() > 1)){
                    result_directoy_list.pop_back();
                }
            }else{
                if (result_directoy_list.back() != '/'){
                    result_directoy_list.append("/");
                }
                result_directoy_list.append(search);

            }

         }



    }

    this->cwd_name = result_directoy_list;
}

void inode_state::ls_all(){
    cwd->contents->out();
}




void inode_state::ls_all_rec(inode_ptr inode){
    if(inode->contents->is_file() != true){
        inode->contents->out();
        vector<inode_ptr> targets = inode->contents->get_children();
        vector<string> target_names = inode->contents->get_children_names();
        for(size_t i = 0; i< targets.size(); i++){
            cout <<"/"<< target_names[i] << endl;
            ls_all_rec(targets[i]);
        }
    }

}


void inode_state::ls_all_rec_command(string& directory_line){
    ls_all_rec(this->process_directory(directory_line));
}

const string& inode_state::prompt() { return prompt_; }
const string& inode_state::pwd_cwd() { return cwd_name; }

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}


file_error::file_error (const string& what):
            runtime_error (what) {
}

bool plain_file::is_file(){
    return true;
}

size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   this->data = words;
   DEBUGF ('i', words);
}

void plain_file::remove (const string&) {
   throw file_error ("is a plain file");
}

void plain_file::add_entry (const string&, inode_ptr) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::mkdir (const string&) {
   throw file_error ("is a plain file");
}

void plain_file::out() {
   for(const auto& p: this->data){
       cout << p << " ";
   }
   cout << endl;
}

inode_ptr plain_file::mkfile (const string&, wordvec&) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::search_entry(string& ){
    throw file_error ("is a plain file");
}

vector<inode_ptr> plain_file::get_children(){
    throw file_error ("is a plain file");
}


vector<string> plain_file::get_children_names(){
    throw file_error ("is a plain file");
}

//directory::directory(inode_ptr parent,inode_ptr self){


//}


bool directory::is_file(){
    return false;
}


size_t directory::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& directory::readfile() const {
   throw file_error ("is a directory");
}

void directory::writefile (const wordvec&) {
   throw file_error ("is a directory");
}


void directory::add_entry (const string& name, inode_ptr targ_inode) {
    auto it = this->dirents.find(name);
      if (it == dirents.end()){
          this->dirents[name] = targ_inode;
      } else{
          throw file_error ("cannot create directory : File exists");
      }

}


void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   auto it = this->dirents.find(dirname);
     if (it == dirents.end()){
         inode_ptr new_pointer =
                 make_shared<inode>(file_type::DIRECTORY_TYPE);
         string curr_dir = ".";
         string parent_dir = "..";
         new_pointer->contents->add_entry(curr_dir,new_pointer);
         new_pointer->contents->add_entry(parent_dir,this->dirents[curr_dir]);
         this->dirents[dirname] = new_pointer;
         return new_pointer;


     } else{
         throw file_error ("cannot create directory : File exists");
     }


   inode_ptr new_pointer = make_shared<inode>(file_type::DIRECTORY_TYPE);
   string curr_dir = ".";
   string parent_dir = "..";
   new_pointer->contents->add_entry(curr_dir,new_pointer);
   new_pointer->contents->add_entry(parent_dir,this->dirents[curr_dir]);
   this->dirents[dirname] = new_pointer;
   return new_pointer;
}

void directory::out() {
   for(const auto& p: this->dirents){

       cout << p.second->inode_nr << "  " << p.second->contents->size() << "  " << p.first;
       if( p.second->contents->is_file() != true){
           cout << "/";
       }
       cout << endl;
   }

}

inode_ptr directory::search_entry(string& entry_string){
    auto it = this->dirents.find(entry_string);
      if (it != dirents.end()){
        return this->dirents[entry_string];
      } else{
          throw file_error ("No such file or directory");
      }

}

inode_ptr directory::mkfile (const string& filename, wordvec& contents) {
    auto it = this->dirents.find(filename);
      if (it == dirents.end()){
      //create file and add it to dirents
      inode_ptr new_pointer = make_shared<inode>(file_type::PLAIN_TYPE);
      new_pointer->contents->writefile(contents);
      this->dirents[filename] = new_pointer;
      } else {
      //update file contents
      this->dirents[filename]->contents->writefile(contents);
      }

    inode_ptr new_pointer = make_shared<inode>(file_type::PLAIN_TYPE);

   DEBUGF ('i', filename);
   return nullptr;
}

vector<inode_ptr> directory::get_children(){
    vector<inode_ptr> result;
    for(const auto& p: this->dirents){
        if((p.first != ".") && (p.first != "..") && (p.second->contents->is_file() != true)){
            result.push_back(p.second);
        }
    }
    return result;

}

vector<string> directory::get_children_names(){
    vector<string> result;
    for(const auto& p: this->dirents){
        if((p.first != ".") && (p.first != "..") && (p.second->contents->is_file() != true)){
            result.push_back(p.first);
        }
    }
    return result;

}

