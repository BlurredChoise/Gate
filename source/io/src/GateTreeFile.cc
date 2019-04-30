//
// Created by mdupont on 12/03/18.
//

#include "GateTreeFile.hh"

#include <iostream>

using namespace std;

GateTree::GateTree()
{

  this->add_size<double>();

  this->add_name<double>("double");

  this->add_size<long double>();
  this->add_size<float>();
//  this->add_size<int>();
  this->add_size<uint8_t>();
  this->add_size<uint16_t>();
  this->add_size<uint32_t>();
  this->add_size<uint64_t>();

  this->add_size<int8_t>();
  this->add_size<int16_t>();
  this->add_size<int32_t>();
  this->add_size<int64_t>();

  this->add_size<bool>();
  this->add_size<char>();


}



const std::string GateTree::type_to_name(std::type_index t_index)
{
    return m_tmapOfName[t_index];
}

void OutputTreeFile::set_tree_name(const std::string &name)
{
  m_nameOfTree = name;
}

OutputTreeFile::~OutputTreeFile()
{
//  cout << "~OutputTreeFile" << endl;
}

OutputTreeFile::OutputTreeFile() : m_nameOfTree("tree")
{}
