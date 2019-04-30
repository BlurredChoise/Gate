//
// Created by mdupont on 13/03/18.
//

#include "GateAsciiFile.hh"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstring> //for strerror
#include <sstream>


#include "GateExceptions.hh"

using namespace std;

void GateAsciiTree::register_variable(std::string name, const void *p, std::type_index t_index)
{
//    std::cout << "AsciiFile::register_variable name = " << name << " t_index = " << t_index.name() << endl;

//  AsciiData d(p, name, t_index, m_tmapOfSave_to_file.at(t_index));
    AsciiData dd(p, name, t_index, m_tmapOfSave_to_file.at(t_index), m_tmapOfRead_from_string.at(t_index));

    for (auto&& d : m_vector_of_pointer_to_data) // access by const reference
    {
        if(d.name() == name)
        {
            string s("Error: Key '");
            s+= name;
            s += "' already used !";
            cerr << s << endl;
            throw new KeyAlreadyExistsException( s );
        }
    }

    m_vector_of_pointer_to_data.push_back(dd);

}

void GateAsciiTree::register_variable(const std::string name, const char *p, size_t )
{
    this->register_variable(name, p, typeid(char*));
}

void GateAsciiTree::register_variable(const std::string name, const std::string *p, size_t )
{
    this->register_variable(name, p, typeid(string));
}

void GateAsciiTree::open(const char *s, std::ios_base::openmode mode)
{

}



bool OutputAsciiTreeFile::is_open()
{
  return m_file.is_open();
}

bool InputAsciiTreeFile::is_open()
{
  return m_file.is_open();
}



void OutputAsciiTreeFile::fill()
{
    uint32_t i = 0;
    for (auto&& d : m_vector_of_pointer_to_data) // access by const reference
    {

        if (d.m_type_index == typeid(char*) )
        {
            const char* p = (const  char *)d.m_pointer_to_data;
            m_file << p;
        }
        else
        {
            d.m_save_to_file(&d, m_file);
        }
        if (i < m_vector_of_pointer_to_data.size() - 1)
            m_file << m_sep;
        i++;
    }
    m_file << "\n";
}
OutputAsciiTreeFile::OutputAsciiTreeFile()
{}

GateAsciiTree::GateAsciiTree() : m_sep(",")
{
    this->attach_read_and_write_function<double>();
    this->attach_read_and_write_function<float>();
    this->attach_read_and_write_function<uint8_t>();
    this->attach_read_and_write_function<uint16_t>();
    this->attach_read_and_write_function<uint32_t>();
    this->attach_read_and_write_function<uint64_t>();

    this->attach_read_and_write_function<int8_t>();
    this->attach_read_and_write_function<int16_t>();
    this->attach_read_and_write_function<int32_t>();
    this->attach_read_and_write_function<int64_t>();

    this->attach_read_and_write_function<bool>();
    this->attach_read_and_write_function<char>();

    this->attach_read_and_write_function<string>();
    this->attach_read_and_write_function<char*>();
}


void OutputAsciiTreeFile::write_header()
{
    if(!m_file.is_open())
        throw ClosedFileException("InputAsciiTreeFile::write_header: try to write in closed file");

    if( (m_mode & ios_base::out) != ios_base::out )
        throw ModeFileException("OutputAsciiTreeFile::write_header: file not opened in write mode"); // Should not happen

    m_file << "#";
    uint32_t i = 0;
    for (auto&& d : m_vector_of_pointer_to_data) // access by const reference
    {
        m_file << d.name();
        if (i < m_vector_of_pointer_to_data.size() - 1)
            m_file << m_sep;
        i++;
    }
    m_file << "\n";
}

AsciiData::AsciiData(const void *pointer_to_data,
                     const string &name,
                     const type_index &type_index,
                     save_to_file_f &save_to_file, read_from_string_f &read_from_string) :
    Data(pointer_to_data,
         name,
         type_index),
    m_save_to_file(save_to_file),
    m_read_from_string(read_from_string)
{}


void OutputAsciiTreeFile::write_variable(std::string name, const void *p, std::type_index t_index)
{
    this->register_variable(name, p, t_index);
}

void OutputAsciiTreeFile::write_variable(const std::string name, const std::string *p, size_t nb_char)
{
    this->register_variable(name, p, nb_char);
}

void OutputAsciiTreeFile::write_variable(const std::string name, const char *p, size_t nb_char)
{
    this->register_variable(name, p, nb_char);
}
void OutputAsciiTreeFile::open(const std::string& s)
{
  GateFile::open(s.c_str(), std::fstream::out);
  m_file.open(s, std::fstream::out);

  if(!m_file.is_open())
  {
    std::stringstream ss;
    ss << "Error opening file! '"  << s <<  "' : " << strerror(errno) ;
    throw std::ios::failure(ss.str());
  }

  m_file << std::setprecision(10);
}

void OutputAsciiTreeFile::close()
{
  m_file.close();
}

void InputAsciiTreeFile::close()
{
  m_file.close();
}


void InputAsciiTreeFile::open(const std::string &s)
{
  GateFile::open(s.c_str(), std::fstream::in);
  m_file.open(s, std::fstream::in);
  m_file << std::setprecision(10);

  if(!m_file.is_open())
  {
    std::stringstream ss;
    ss << "Error opening file! '"  << s <<  "' : " << strerror(errno) ;
    throw std::ios::failure(ss.str());
  }
    // get length of file:
    m_file.seekg (0, m_file.end);
    m_lenght_of_file = m_file.tellg();
    m_file.seekg (0, m_file.beg);
}


vector<string> split_string(const string &line, const string &sep)
{
    std::string s = line;
    vector<string> ret;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(sep)) != std::string::npos) {
        token = s.substr(0, pos);
        cout << "token = " << token << " ";
        ret.push_back(token);
        s.erase(0, pos + sep.length());
    }
    cout << endl;
    return ret;
}


void InputAsciiTreeFile::read_header()
{
    if(!is_open())
        throw ClosedFileException("InputAsciiTreeFile::read_header: try to read from closed file");

    string                line;
    getline(m_file,line);

    size_t pos_start = line.find_last_of('#');

    if (pos_start != string::npos)
        line = line.substr(pos_start + 1);
    else
        throw MissingHeaderException("InputAsciiTreeFile::read_header: no header found in file");

    m_list_from_header = split_string(line, m_sep);

    if(!m_list_from_header.size())
        throw MalFormedHeaderException("InputAsciiTreeFile::read_header: empty header found in file");
}


void InputAsciiTreeFile::read_entrie()
{
    string line;

    getline(m_file,line);

    cout << "line read = " << line << endl;
    auto vs = split_string(line, m_sep);

    for (auto&& d : m_vector_of_pointer_to_data){
        cout << "search variable = " << d.name() <<  " index = " <<  d.m_index_of_this_data_in_header <<   endl;
        string s = vs.at(d.m_index_of_this_data_in_header);
        d.m_read_from_string(&d, s);
    }

}
void InputAsciiTreeFile::read_variable(const std::string name, void *p, std::type_index t_index)
{
    cout << "read_variable, nam = " << name << endl;

    for (auto variable : m_list_from_header){

    }

    for(auto i = 0; i < m_list_from_header.size(); ++i)
    {
        auto name_from_header  = m_list_from_header[i];
        if (name_from_header == name)
        {
            register_variable(name, p, t_index);
            auto d = m_vector_of_pointer_to_data.back();
            m_vector_of_pointer_to_data.pop_back();
            d.m_index_of_this_data_in_header = i;
            m_vector_of_pointer_to_data.push_back(d);
        }
    }


}
bool InputAsciiTreeFile::data_to_read()
{
    throw std::runtime_error("Not implemented");
    return false;
}


bool OutputAsciiTreeFile::s_registered =  GateOutputTreeFileFactory::_register(OutputAsciiTreeFile::_get_factory_name(), &OutputAsciiTreeFile::_create_method);


