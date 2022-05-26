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

  /* code for opening and reading through a directory adapted from May 17 tutorial code */
  DIR* dir;
  dir = opendir(dir_name.c_str());
  //open the current directory
  if(!dir) {
    std::cout<<"Unable to open directory"<<std::endl;
    exit(1);
  }

  struct dirent* dirContent;
  bool isDir;

  while((dirContent = readdir(dir)) != NULL) {
    isDir = (dirContent->d_type == DT_DIR);
    //check if the current content is a directory

    if(isDir) {
      if(TRIVIAL_DIR(dirContent->d_name)) {
        continue;
        //if the current item is a trivial directory, skip this iteration
      }

      res.n_dirs++;
      //increment the number of other directories

      chdir(dir_name.c_str());
      //change the working directory to the new folder

      Results dirRes = getDirStats(dirContent->d_name, n);
      //recursively get the directory stats for any sub-directories

      if(dirRes.largest_file_size > res.largest_file_size) {
        //if the sub-directory found a larger file, update the results
        res.largest_file_size = dirRes.largest_file_size;
        std::string longestPath = dirRes.largest_file_path;
        longestPath.insert(0, "/");
        longestPath.insert(0, dir_name);
        res.largest_file_path = longestPath;
      }

      res.n_dirs += dirRes.n_dirs;
      res.n_files += dirRes.n_files;
      res.all_files_size += dirRes.all_files_size;
      //add the stats found in the sub-directory
    }
    else {
      chdir(dir_name.c_str());
      res.n_files++;
      //change to the current directory and increment the number of files

      char pathname[4096];
      getcwd(pathname, 4096);
      //get the pathname to the current spot

      std::string full_name = pathname;
      full_name.append("/");
      full_name.append(dirContent->d_name);
      //add the file name to the end of the path

      struct stat stats;
      int result = stat(full_name.c_str(), &stats);
      if(result != 0) {
        exit(1);
      }
      //use stat to get data about the file

      chdir("..");
      res.all_files_size += stats.st_size;
      //return to the previous directory and add to the total file size

      if(stats.st_size > res.largest_file_size) {
        res.largest_file_size = stats.st_size;
        std::string long_path = dir_name;
        long_path.append("/");
        long_path.append(dirContent->d_name);
        res.largest_file_path = long_path;
        //if the current file is the largest, adjust accordingly
      }
    }
  }

  closedir(dir);
  chdir("..");
  return res;
}
