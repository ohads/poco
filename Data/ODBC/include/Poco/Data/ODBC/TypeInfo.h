//
// TypeInfo.h
//
// $Id: //poco/Main/Data/ODBC/include/Poco/Data/ODBC/TypeInfo.h#3 $
//
// Library: ODBC
// Package: ODBC
// Module:  TypeInfo
//
// Definition of TypeInfo.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef ODBC_DataTypes_INCLUDED
#define ODBC_DataTypes_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/NamedTuple.h"
#include "Poco/DynamicAny.h"
#include <vector>
#include <map>
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqlext.h>


namespace Poco {
namespace Data {
namespace ODBC {


class ODBC_API TypeInfo
	/// Datatypes mapping utility class.
	///
	/// This class provides mapping between C and SQL datatypes as well
	/// as datatypes supported by the underlying database. In order for database
	/// types to be available, a valid conection handle must be supplied at either
	/// object construction time, or at a later point in time, through call to
	/// fillTypeInfo member function.
	///
	/// Class also provides a convenient debugging function that prints
	/// tabulated data to an output stream.
{
public:
	typedef std::map<int, int> DataTypeMap;
	typedef DataTypeMap::value_type ValueType;
	typedef Poco::NamedTuple<std::string,
		SQLSMALLINT,
		SQLINTEGER,
		std::string,
		std::string,
		std::string,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		std::string,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLSMALLINT,
		SQLINTEGER,
		SQLSMALLINT> TypeInfoTup;
	typedef std::vector<TypeInfoTup> TypeInfoVec;

	explicit TypeInfo(SQLHDBC* pHDBC=0);
		/// Creates the TypeInfo.

	~TypeInfo();
		/// Destroys the TypeInfo.

	int cDataType(int sqlDataType) const;
		/// Returns C data type corresponding to supplied SQL data type.

	int sqlDataType(int cDataType) const;
		/// Returns SQL data type corresponding to supplied C data type.

	void fillTypeInfo(SQLHDBC pHDBC);
		/// Fills the data type info structure for the database.

	DynamicAny getInfo(SQLSMALLINT type, const std::string& param) const;
		/// Returns information about specified data type as specified by parameter 'type'.
		/// The requested information is specified by parameter 'param'.

	void print(std::ostream& ostr);
		/// Prints all the types (as reported by the underlying database)
		/// to the supplied output stream.

private:
	void fillCTypes();
	void fillSQLTypes();

	DataTypeMap _cDataTypes; 
	DataTypeMap _sqlDataTypes; 
	TypeInfoVec _typeInfo;
	SQLHDBC*    _pHDBC;
};


inline DynamicAny TypeInfo::getInfo(SQLSMALLINT type, const std::string& param) const
{
	TypeInfoVec::const_iterator it = _typeInfo.begin();
	TypeInfoVec::const_iterator end = _typeInfo.end();
	for (; it != end; ++it)
	{
		if (type == it->get<1>())
			return (*it)[param];
	}

	throw NotFoundException(param);
}


} } } // namespace Poco::Data::ODBC


#endif
