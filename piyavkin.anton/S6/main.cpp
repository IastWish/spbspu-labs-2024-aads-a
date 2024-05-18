#include <iostream>
#include <string>

#include "timsort.hpp"
#include "createcontainer.hpp"

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "File entered incorrectly\n";
    return 1;
  }
  size_t size = std::stoull(argv[3]);
  using namespace piyavkin;
  if (argv[2] == "ints")
  {
    std::forward_list< int > list;
    List< int > bi_list;
    std::deque< int > deque;
    create_container(deque, bi_list, list, n); 
  }
}
