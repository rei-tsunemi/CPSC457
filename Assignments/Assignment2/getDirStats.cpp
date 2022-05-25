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

#define TRIVIAL_DIR(buf) (((buf)[0] == '.' && (buf)[1] == '\0') || ((buf)[0] == '.' && (buf)[1] == '.' && (buf)[2] == '\0'))

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
  // The results below are all hard-coded, to show you all the fields
  // you need to calculate. You should delete all code below and
  // replace it with your own code.
  Results res;

  res.n_dirs = 0;
  res.n_files = 0;
  //set the number of files and directories to 0 to start

  res.largest_file_path = "";
  res.all_files_size = 0;
  res.largest_file_size = 0;
  //also initilalize the values for other Results values

  std::vector<std::string> d_names;
  d_names.insert(0, dir_name);

  while(!d_names.empty()) {
    std::string name = d_names.pop_back();
    std::cout<<name<<std::endl;
    DIR* dir;
    dir = opendir(name.c_str());
    //open the current directory

    struct dirent* dirContent;
    bool isDir;

    while((dirContent = readdir(dir)) != NULL) {
      isDir = dirContent->d_type == DT_DIR;

      if(isDir) {
        if(TRIVIAL_DIR(dirContent->d_name)) {
        continue;
        }

        res.n_dirs++;
        std::string sub_name = dirContent->d_name;
        d_names.insert(0, sub_name);
      }
      else {
        res.n_files++;
        //char pathname[4096];
        //getcwd(pathname, 4096);
        std::cout<<"file: "<<dirContent->d_name<<std::endl;
      }
    }
  }

  return res;

  /*
  res.n_dirs = 0;
  res.n_files = 0;
  //set the number of files and directories to 0 to start

  res.largest_file_path = "";
  res.all_files_size = 0;
  res.largest_file_size = 0;
  //also initilalize the values for other Results values

  /* code for opening and reading through a directory adapted from May 17 tutorial code *
  DIR* dir;
  dir = opendir(dir_name.c_str());
  //open the current directory

  struct dirent* dirContent;
  bool isDir;

  while((dirContent = readdir(dir)) != NULL) {
    isDir = (dirContent->d_type == DT_DIR);

    if(isDir) {
      if(TRIVIAL_DIR(dirContent->d_name)) {
        continue;
      }
      std::cout<<dirContent->d_name<<std::endl;

      res.n_dirs++;
      char* name = dirContent->d_name;
      closedir(dir);
      Results dirRes = getDirStats(dirContent->d_name, n);
      //recursively get the directory stats for any sub-directories

      if(dirRes.largest_file_size > res.largest_file_size) {
        //if the sub-directory found a larger file, update the results
        res.largest_file_size = dirRes.largest_file_size;
        std::string longestPath = dirRes.largest_file_path;
        longestPath.insert(0, dirContent->d_name);
        res.largest_file_path = longestPath;
      }

      res.n_dirs += dirRes.n_dirs;
      res.n_files += dirRes.n_files;
      //add the stats found in the sub-directory
    }
    else {
      res.n_files++;
      //char pathname[4096];
      //getcwd(pathname, 4096);
      std::cout<<"file: "<<dirContent->d_name<<std::endl;
    }
  }

  closedir(dir);
  */

  /*
  if (! is_dir(dir_name)) return res;

  // prepare a fake results
  res.largest_file_path = dir_name + "/some_dir/some_file.txt";
  res.largest_file_size = 123;
  res.n_files = 321;
  res.n_dirs = 333;
  res.all_files_size = 1000000;

  std::string type1 = "C source";
  int count1 = 5;
  res.most_common_types.push_back({ type1, count1 });
  res.most_common_types.push_back({ "makefile script", 4 });
  res.most_common_types.push_back({ "C++ source", 2 });
  res.most_common_types.push_back({ "PNG image", 1 });

  res.most_common_words.push_back({ "hello", 3 });
  res.most_common_words.push_back({ "world", 1 });

  std::vector<std::string> group1;
  group1.push_back(dir_name + "/file1.cpp");
  group1.push_back(dir_name + "/lib/sub/other.c");
  res.duplicate_files.push_back(group1);
  std::vector<std::string> group2;
  group2.push_back(dir_name + "/readme.md");
  group2.push_back(dir_name + "/docs/readme.txt");
  group2.push_back(dir_name + "/x.y");
  res.duplicate_files.push_back(group2);
  */

}
