#include "commands.hpp"
#include <stdexcept>

void novokhatskiy::unionCmd(mainMap& dict, std::istream& in, std::ostream&)
{
  std::string resName = {};
  std::string first = {};
  std::string second = {};
  in >> resName >> first >> second;
  map resDict;
  map dict1 = dict.at(first);
  map dict2 = dict.at(second);
  for (const auto& key1 : dict1)
  {
    resDict.insert2(key1);
  }
  for (const auto& key2 : dict2)
  {
    if (resDict.find(key2.first) == resDict.cend())
    {
      resDict.insert2(key2);
    }
  }
  dict[resName] = resDict;
  /*auto begin1 = dict1.cbegin();
  auto end1 = dict1.cend();
  while (begin1 != end1)
  {
    resDict.insert({ begin1->first, begin1->second });
    begin1++;
  }
  auto begin2 = dict2.cbegin();
  auto end2 = dict2.cend();
  while (begin2 != end2)
  {
    resDict.insert({ begin2->first, begin2->second });
    begin2++;
  }

  if (dict.find(resName) != dict.end())
  {
    dict.erase(resName);
  }
  dict.insert2({ resName, resDict });*/
}

void novokhatskiy::intersectCmd(mainMap& dict, std::istream& in, std::ostream&)
{
  std::string resName = {};
  std::string first = {};
  std::string second = {};
  in >> resName >> first >> second;
  map resDict;
  map dict1 = dict.at(first);
  map dict2 = dict.at(second);
  auto begin = dict1.cbegin();
  auto end = dict1.cend();
  while (begin != end)
  {
    if (dict2.find(begin->first) != dict2.cend())
    {
      resDict.insert(*begin);
    }
    begin++;
  }
  if (dict.find(resName) != dict.cend())
  {
    dict.at(resName).clear();
    dict.at(resName) = std::move(resDict);
  }
  else
  {
    dict[resName] = resDict;
    //dict.insert(std::make_pair< std::string, map >(resName, resDict) );
  }
}

void novokhatskiy::complement(mainMap& dict, std::istream& in, std::ostream&)
{
  std::string resName = {};
  std::string first = {};
  std::string second = {};
  in >> resName >> first >> second;
  map resDict;
  map dict1 = dict.at(first);
  map dict2 = dict.at(second);

  auto begin = dict1.cbegin();
  auto end = dict1.cend();
  while (begin != end)
  {
    if (dict2.find(begin->first) == dict2.cend())
    {
      resDict.insert(*begin);
    }
    begin++;
  }
  begin = dict2.cbegin();
  end = dict2.cend();
  while (begin != end)
  {
    if (dict1.find(begin->first) == dict1.cend())
    {
      resDict.insert(*begin);
    }
    begin++;
  }
  if (dict.find(resName) != dict.cend())
  {
    dict.at(resName).clear();
    dict.at(resName) = std::move(resDict);
  }
  else
  {
    dict[resName] = resDict;
    //dict.insert({ resName, resDict });
  }
}

void novokhatskiy::print(mainMap& dict, std::istream& in, std::ostream& out)
{
  std::string nameOfDict = {};
  in >> nameOfDict;
  map tmp = dict.at(nameOfDict);
  if (tmp.empty())
  {
    throw std::logic_error("<EMPTY>");
  }
  out << nameOfDict;
  for (auto i = tmp.cbegin(); i != tmp.cend(); i++)
  {
    out << ' ' << (*i).first << ' ' << (*i).second;
  }
  out << '\n';
}
