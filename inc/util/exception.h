#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

class Exception: public std::exception
{
public:
	std::string sWhat;

	Exception(std::string sWhat)
			:
		sWhat(sWhat)
	{}//constructor

  virtual const char* what() const throw()
  {
    return sWhat.c_str();
  }
};//class

#endif
