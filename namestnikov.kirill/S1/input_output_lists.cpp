#include "input_output_lists.hpp"
#include <string>
#include <limits>
#include <iostream>

void namestnikov::inputLists(std::istream & in, ForwardList<pair_t> & dataList)
{
  std::string listParameters = "";
  in >> listParameters;
  while (in)
  {
    dataList.push_front({listParameters, ForwardList<unsigned long long>{}});
    while ((in >> listParameters) && (std::isdigit(listParameters[0])))
    {
      unsigned long long number = std::stoull(listParameters);
      dataList.front().second.push_front(number);
    }
  }
}

void namestnikov::outputNames(std::ostream & out, ForwardList<pair_t> & dataList, size_t & maxSize)
{
  dataList.reverse();
  for (ForwardIterator<pair_t> fwdIt = dataList.begin(); fwdIt != dataList.end(); ++fwdIt)
  {
    maxSize = std::max(maxSize, fwdIt->second.max_size());
    fwdIt->second.reverse();
    fwdIt != dataList.begin() ? out << " " << fwdIt->first : out << fwdIt->first;
  }
  out << "\n";
}

void namestnikov::outputNumsAndSums(std::ostream & out, ForwardList<pair_t> & dataList, size_t maxSize)
{
  ForwardList<unsigned long long> sumsList;
  unsigned long long sum = 0;
  bool gotOverflow = false;
  const unsigned long long MAX_SUM = std::numeric_limits<unsigned long long>::max();
  for (size_t i = 0; i < maxSize; ++i)
  {
    for (auto it = dataList.begin(); it != dataList.end(); ++it)
    {
      ForwardIterator<unsigned long long> numIt = it->second.begin();
      size_t currentListSize = it->second.max_size();
      if (i < currentListSize)
      {
        std::advance(numIt, i);
        unsigned long long number = *numIt;
        sum != 0 ? out << " " << number : out << number;
        if (MAX_SUM - sum < number)
        {
          gotOverflow = true;
        }
        sum += number;
      }
    }
    if (sum != 0)
    {
      out << "\n";
    }
    sumsList.push_front(sum);
    sum = 0;
  }
  if (gotOverflow)
  {
    throw std::overflow_error("The number is too big");
  }
  sumsList.reverse();
  for (ForwardIterator<unsigned long long> sumIt = sumsList.begin(); sumIt != sumsList.end(); ++sumIt)
  {
    sumIt != sumsList.begin() ? out << " " << *sumIt : out << *sumIt;
  }
  out << "\n";
}

bool namestnikov::haveNumbers(ForwardList<pair_t> & dataList)
{
  bool gotNumbers = false;
  for (auto it = dataList.begin(); it != dataList.end(); ++it)
  {
    gotNumbers = gotNumbers || !(it->second.empty());
  }
  return gotNumbers;
}

void namestnikov::outputLists(std::ostream & out, ForwardList<pair_t> & dataList)
{
  if (dataList.empty())
  {
    throw std::invalid_argument("List is empty");
  }
  size_t maxSize = 0;
  outputNames(out, dataList, maxSize);
  if (!haveNumbers(dataList))
  {
    throw std::invalid_argument("Got no numbers");
  }
  outputNumsAndSums(out, dataList, maxSize);
}
