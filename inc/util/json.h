#ifndef JSON_H
#define JSON_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <fstream>
#include "util/exception.h"

class Json
{
public:
	boost::property_tree::ptree tree;
	const std::string fileName;

	Json(const std::string fileName)
			:
		tree(),
		fileName(std::string(fileName).append(".json"))
	{
		//make shure the file exists before loading it.
		std::ifstream f(this->fileName.c_str());
		if(!f.good())
			return;
		boost::property_tree::read_json(this->fileName, tree);
	}//construcor

	~Json()
	{
		if(!fileName.empty())
			boost::property_tree::write_json(fileName, tree);
	}//destructor

	template<class T>
	inline T get(std::string what)
	{
		try
		{
			return tree.get<T>(what);
		}//try
		catch (...)
		{
			throw Exception(std::string("cannot find setting: ").append(what));
		}///catch
	}//function

	inline std::vector<std::string> get_vec(std::string what)
	{
		try
		{
			std::vector<std::string> r;
			for (auto& item : tree.get_child(what))
				r.push_back(item.second.get_value<std::string>());
			return r;
		}//try
		catch (...)
		{
			throw Exception(std::string("cannot find setting: ").append(what));
		}///catch
	}//function
};//class


#endif