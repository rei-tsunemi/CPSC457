# Agenda
- talk about C++ today ...
- *NOTE:* this file was my script, and I have talked about most of these things out loud in tutorial. Please also refer to the code ...
    - it is not a very organized introduction to C++, because during tutorial I essentially pointed to references and man pages, but that is actually how I recommend you to learn things!
        - I believe giving examples of how to use functions (i.e. `push_back()` in `std::vector`) is pointless, becaues you can read the reference yourself, and the reference usually gives a very nice example that is way better than my example
        - If you think the TA (i.e. me) is not doing a good job because the TA points you to reading the references (and googling) rather than actually "teaching", then I'm terribly sorry about that (you are probably right though). But honestly, that is how I studied programming, and my experience with TAs is that they are usually not very helpful, especially when they talk about things that can be easily found inside a manual/reference. I would like to spend tutorial time on other things that (hopefully) are not included or not easily obtained by reading references.
        - I hope the structure of the future tutorials will be better from now on, since I will actually need to cover specific things, and not just "introduce C++"

# C++ programming
- compile:
    - we will use the `g++` compiler (there are other compilers such as `clang`, but we will not use it in this course)
    - `-Wall` turns on a lot of warnings
    - `-Wextra` turns on a more warnings
    - `-Werror` will make the warnings errors
    - `-O2` is an optimization level (there is `O1`, `O2`, `O3`, and probably more)
```
g++ -Wall -Werror -O2 <filename> -o <outputname>
```
- IO with `std::cin` and `std::cout`, or `scanf` and `printf` 
    - be consistent (i.e. choose either `std::cout` or `printf`)! Otherwise your output might be mixed up, since `std::cout` and `printf` will buffer the outputs

- arrays
    - just try not to really use them ... 1D is fine, but 2D is more cancer
    - use vector instead for simplicity

- Be careful with `char*` strings
    - if do `char str[] = "hello"`, then `str` is a normal (readable and writable) array
    - if do `char* str = "hello"`, then `str` is READ ONLY MEMORY!
        - how to debug segfaults?
        - compile with debug `-g` and run it through `gdb` or `strace` (usually `gdb` is good enough)
        - do `printf` and then `fflush(stdout)` as break points
    - the string array is always 1 more size since it needs to store the null terminator `\0` ... just use strings for simplicity
        - can print the first `n` chars of string with `printf("%.*s", n, string)`

- if/else
    - 1 line no need braces
- while, do while, for loop
    - break/continue
    - `++i` vs `i++` (`i++` will return value and then increment it)
    - iterators with auto type is ok, better to use `const auto&` if you don't want to modify it
- switch
- function declaration is what you think it is

- pointer vs reference 
    - reference is essentially a pointer
        - reference must be initialized and cannot be changed
    - dereference pointers with `ptr->stuffToDereference`

- input args to main with 
```
int main(int argc, char * argv[])
```
    - can print out the argc and argvs out

- classes and structs
    - same except struct is default public, class is default private

- undefined behaviour
    - access array out of bounds
    - return address on stack

- templates (do swap as example)
```
template<class T>
void swap(T& a, T& b){
    T temp = a;
    a=b;
    b=temp;
}
```
if you want to use move semantics, you can write
```
template<class T>
void swap(T&& a, T&& b){
    T temp(std::move(a));
    a=std::move(b);
    b=std::move(temp);
}
```
- a great article explaining move semantics (read it 2 years ago, but only read half of it before tut, so will not really explain it ...): http://thbecker.net/articles/rvalue_references/section_01.html

- vector
- list
- (ordered) set
- unordered_map (hashmap)

- constructors:
    - default initialization might be confusing ...
    - in C, declaration is simply declaration... the memory is not initialized
    - in C++, primitive types are not initialized (because they have no constructors), but (usually non-static) class types are initialized
        - can show in compiler explorer on what is initialized/done in assembly: 
```
class A{
    int a;
    int b[3];
    std::string c;
    std::string d[3];
};

int main(){
    A a; //default initialization
    A b{}; //zero initialization
}
```
        - https://en.cppreference.com/w/cpp/language/default_initialization

- optimization example:
    - instead of incrementing a global variable (which must require moving the value from the memory address of the global variable) many times, you can create an integer `temp` and increment `temp` instead. At the end, set `temp` to `global.d`.
    - inside compiler explorer, we will see that if we don't use `temp`, there will be memory moving with the `mov` assembly command inside the loop. If we use `temp`, then no memory moving with `mov` is present inside the loop.
```
struct A{
    long long d;
};

struct A global;

int main(){
  int r;
  double rsq = double(r) * r; 
 
 long long temp=0;

  for( double x = -r ; x <= r ; x ++) 
    for( double y = -r ; y <= r ; y ++) 
      if( x*x + y*y <= rsq)  
        //global.d ++; 
        temp ++; 
 
  global.d=temp;
}
```

- brace initializer ... does zero initialization
    - https://en.cppreference.com/w/cpp/language/zero_initialization
        - if its a class, the definition is recursive
    - good link that shows lots of possibilities: https://stackoverflow.com/questions/8280023/does-a-c-struct-have-a-default-constructor

- for file reading:
    - read out all the contents of the file 
        - look at man page to say when EOF ... 
    - `    int fd = open(pathname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );` opens file if not exist with appropriate permissions set
    - create a `char buffer [someSize]`
    - see if can read anything. 0 means EOF and -1 means error (consult man page!)

- there is C++ streams, but will not cover here
    - they are more slow, and very abstracted away from system calls ...

- global variables
    - really means static variables not inside functions
    - static variables inside function is just 1 occurrence of the variable in function ...
    - static functions is a function that is only visible inside the same file (translation unit)
        - normal non-static functions are visible throughout the program (across different files)
        - to use functions from other files, either include header, or use `extern`
        - but if the function is declared as static, extern doesn't help ... you can't access it
        - same behaviour with global (non-static variables)
    - https://stackoverflow.com/questions/572547/what-does-static-mean-in-c
    - there exists something called anonymous namespace, essentially same as static, except you can also declare own types
        - https://stackoverflow.com/questions/154469/unnamed-anonymous-namespaces-vs-static-functions
        - https://stackoverflow.com/questions/4422507/superiority-of-unnamed-namespace-over-static


