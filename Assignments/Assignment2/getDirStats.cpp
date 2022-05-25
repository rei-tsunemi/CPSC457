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
#include <iostream>

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
  //since the top directory is not included in the number of directories,
  //set both the number of files and number of directories to 0 to begin

  res.largest_file_size = 0;
  res.all_files_size = 0;
  //also initialize the largest and total file size to 0

  res.largest_file_path = "";

  DIR* directory;
  directory = opendir(".");
  struct dirent* contents;
  contents = readdir(directory);

  std::cout << "Point 1" << std::endl;

  while(contents != NULL) {
    char* pathname;
    pathname = getcwd(pathname, 0);
    bool isDir = is_dir(pathname);
    //check if the current directory entry is another directory
    std::cout << "Point 2" << std::endl;

    if(isDir) {
      std::string name = contents->d_name;
      bool isTrivial = (name.compare(".") == 0) || ( name.compare("..") == 0);
      //if the current directory entry is another directory, first check if it is trivial or not

      if(isTrivial) {
        continue;
        //if it is a trivial directory, skip this iteration of the loop
      }
      
      res.n_dirs++;
      //since a non trivial directory was found, increment the number of directories

      Results dirRes = getDirStats(name, n);
      //recursively get the directory stats for this sub-directory

      if(dirRes.largest_file_size > res.largest_file_size) {
        res.largest_file_size = dirRes.largest_file_size;
        //if the sub-directory held a larger file than the previous largest file,
        //change the largest_file_size value

        std::string longestPath = dirRes.largest_file_path;
        longestPath.insert(0, name);
        //insert the current directory name to the largest file path
      }
      
      res.n_dirs += dirRes.n_dirs;
      res.n_files += dirRes.n_files;
      //add the number of files and directories the directory has
      //to the current values
    }
    else {
      std::cout << "Point 3" << std::endl;

      res.n_files++;
      
      struct stat* buf;
      int read = stat(pathname, buf);
      //attemt to read the file data using stat
      if(read != 0) {
        std::cout << "Error reading file" << std::endl;
        exit(1);
        //if the read fails, terminate the program
      }

      std::cout << "Point 4" << std::endl;


      if(buf->st_size > res.largest_file_size) {
        //if the current file is largest than the previous largest file,
        //replace the largest file data
        res.largest_file_size = buf->st_size;
        res.largest_file_path = contents->d_name;
      }      

      res.all_files_size += buf->st_size;
      //add the current file size to the total file size

      
    }

    contents = readdir(directory);
  }

  closedir(directory);

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

  return res;
}
