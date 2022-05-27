/// =========================================================================
/// Written by pfederl@ucalgary.ca in 2020, for CPSC457.
/// =========================================================================
/// You need to edit this file.
///
/// You can delete all contents of this file and start from scratch if
/// you wish, as long as you implement the getDirStats() function as
/// defined in "getDirStats.h".

#include "getDirStats.h"
#include "digester.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#define TRIVIAL_DIR(buf) (((buf)[0] == '.' && (buf)[1] == '\0') || ((buf)[0] == '.' && (buf)[1] == '.' && (buf)[2] == '\0'))

struct MyComp {
    bool operator()(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) const {
        return a.second > b.second;
    }
};

/*
 * Code for using popen and file to get the file type adapted from 
 * https://gitlab.com/cpsc457/public/popen-example 
 */
std::string getFileType(const std::string name);

static bool is_dir(const std::string & path)
{
  struct stat buff;
  if (0 != stat(path.c_str(), & buff)) return false;
  return S_ISDIR(buff.st_mode);
}

// ======================================================================
// You need to re-implement this function !!!!
// ======================================================================
//
// getDirStats() computes stats about directory a directory
//   dir_name = name of the directory to examine
//   n = how many top words/filet types/groups to report
//
// if successful, results.valid = true
// on failure, results.valid = false
//
Results getDirStats(const std::string & dir_name, int n)
{
  Results res;
  std::unordered_map<std::string, int> f_types;

  res.n_dirs = 0;
  res.n_files = 0;
  //set the number of files and directories to 0 to start

  res.largest_file_path = "";
  res.all_files_size = 0;
  res.largest_file_size = 0;
  //also initilalize the values for other Results values

  std::vector<std::string> directories;
  directories.push_back(dir_name);
  //create a stack for traversing through the directories and push the starting dir_name to it

  while (!directories.empty()) {
    std::string d_name = directories.back();
    directories.pop_back();
    //take the top entry out of the stack

    /* code for opening and reading through a directory adapted from May 17 tutorial code */
    DIR* dir;
    dir = opendir(d_name.c_str());
    //open the current directory
    if(!dir) {
      std::cout<<"Unable to open directory"<<std::endl;
      exit(1);
    }

    struct dirent* dirContent;

    while((dirContent = readdir(dir)) != NULL) {
      if(dirContent->d_type == DT_DIR) {
        if(TRIVIAL_DIR(dirContent->d_name)) {
          continue;
          //if the current item is a trivial directory, skip this iteration
        }

        res.n_dirs++;
        std::string full_name = d_name;
        full_name.append("/");
        full_name.append(dirContent->d_name);
        directories.push_back(full_name);
        //increment the number of directories found and add the pathname starting at dir_name to the directories stack
      }
      else if (dirContent->d_type == DT_REG) {
        res.n_files++;

        std::string full_name = d_name;
        full_name.append("/");
        full_name.append(dirContent->d_name);
        //add the file name to the end of the path

        struct stat stats;
        int result = stat(full_name.c_str(), &stats);
        if(result != 0) {
          std::cout<<"unable to get file stats"<<std::endl;
          exit(1);
        }
        //use stat to get data about the file

        res.all_files_size += stats.st_size;

        if(stats.st_size > res.largest_file_size) {
          res.largest_file_size = stats.st_size;
          std::string long_path = d_name;
          long_path.append("/");
          long_path.append(dirContent->d_name);
          res.largest_file_path = long_path;
          //if the current file is the largest, adjust accordingly
        }

        std::string filetype = getFileType(full_name);
        //use a helper function to get the file type

        f_types[filetype] ++;

      }
    }

    closedir(dir);
  }
  
  //code for sorting a map adapted from method 1 on https://gitlab.com/cpsc457/public/word-histogram/-/blob/master/main.cpp
  std::vector<std::pair<std::string, int>> sorted_filetypes;
  for(auto & i: f_types) {
    sorted_filetypes.emplace_back(i.first, i.second);
  }

  if(sorted_filetypes.size() > size_t(n)) {
    std::partial_sort(sorted_filetypes.begin(), sorted_filetypes.begin() + n, sorted_filetypes.end(), MyComp{});
    sorted_filetypes.resize(n);
  }
  else {
    std::sort(sorted_filetypes.begin(), sorted_filetypes.end(), MyComp{});
  }

  res.most_common_types = sorted_filetypes;

  return res;
}

std::string getFileType(const std::string name) {
  std::string command = "file -b " + name;
  FILE* fp = popen(command.c_str(), "r");
  if(fp == nullptr) {
    std::cout<<"popen() failed. Program terminating."<<std::endl;
    exit(1);
  }

  char res[4096], c;
  int i = 0;
  while((c = fgetc(fp)) != EOF) {
    res[i] = c;
    i++;
  }
  res[i] = '\0';
  pclose(fp);

  std::string retVal;
  if(res[0] == '\0') {
    retVal = "file(1) failed for this file";
    //if file(1) failed name the file type accordingly
  }
  else {
    int end = 0;
    while(res[end] != ',' && res[end] != '\n' && res[end] != 0) {
      end++;
      //increment the endline value until the end of the filetype is found
    }
    res[end] = '\0'; //add the end to the string
    retVal = res;
  }

  return retVal;
}
