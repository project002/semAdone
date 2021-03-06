/*
 * CException.h
 *
 *  Created on: Dec 10, 2013
 *      Author: project
 */

#ifndef CEXCEPTION_H_
#define CEXCEPTION_H_

#include <exception>
#include <string>

class CException: public std::exception
{
public:
	CException();
	CException(const std::string &msg);
	virtual const char* what() const throw(); //prototype copied form the internet
	~CException() throw() {};
private:
	std::string _msg;
};

#endif /* CEXCEPTION_H_ */
