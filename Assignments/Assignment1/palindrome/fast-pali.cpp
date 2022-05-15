#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>

#define MB (1024 * 1024)

// split string p_line into a vector of strings (words)
// the delimiters are 1 or more whitespaces
std::vector<std::string>
split( const std::string & p_line)
{
  auto line = p_line + " ";
  std::vector<std::string> res;
  bool in_str = false;
  std::string curr_word = "";
  for( auto c : line) {
    if( isspace(c)) {
      if( in_str)
        res.push_back(curr_word);
      in_str = false;
      curr_word = "";
    }
    else {
      curr_word.push_back(c);
      in_str = true;
    }
  }
  return res;
}

// global variable used in read_char_fast()
char buffer[MB];
int buff_size = 0;
int buff_pos = 0;
// global variables takn from lines 20-22 of fast-int.cpp from
  // https://gitlab.com/cpsc457p21/longest-int-my-getchar 


// fast reading characters adaped from lines 28-40 of fast-int.cpp
  // from https://gitlab.com/cpsc457p21/longest-int-my-getchar 
char read_char_fast() {
  if(buff_pos >= buff_size) {
    buff_size = read(STDIN_FILENO, buffer, MB);

    if(buff_size <= 0) return '\n';

    buff_pos = 0;
  }

  return buffer[buff_pos++];
}

// global variable for stdin_readline()
char buff;

// reads in a line from STDIN
// reads until \n or EOF and result includes \n if present
// returns empty string on EOF
std::string stdin_readline()
{
  std::string result;
  while( 1 == read( STDIN_FILENO, & buffer, 1)) {
    result.push_back(buff);
    if( buff == '\n') break;
  }
  return result;
}

// returns true if a word is palindrome
// palindrome is a string that reads the same forward and backward
//    after converting all characters to lower case
bool
is_palindrome( const std::string & s)
{
  for( size_t i = 0 ; i < s.size() / 2 ; i ++)
    if( tolower(s[i]) != tolower(s[s.size()-i-1]))
      return false;
  return true;
}

// returns the longest palindrome on standard input
// in case of ties for length, returns the first palindrome
// all input is broken into words, and each word is checked
// word is a sequence of characters separated by white space
// white space is whatever isspace() says it is
//    i.e. ' ', '\n', '\r', '\t', '\n', '\f'
std::string
get_longest_palindrome()
{
  std::string max_pali;
  while(1) {
    std::string line = stdin_readline();
    if( line.size() == 0) break;
    auto words = split( line);
    for( auto word : words) {
      if( word.size() <= max_pali.size()) continue;
      if( is_palindrome(word))
        max_pali = word;
    }
  }
  return max_pali;
}

int
main()
{
  std::string max_pali = get_longest_palindrome();
  printf("Longest palindrome: %s\n", max_pali.c_str());
  return 0;
}

