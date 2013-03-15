/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <boost/thread/tss.hpp>

namespace icinga
{

/**
 * Base class for all exceptions.
 *
 * @ingroup base
 */
class I2_BASE_API Exception //: public virtual exception
{
public:
	static StackTrace *GetLastStackTrace(void);
	static void SetLastStackTrace(const StackTrace& trace);

private:
	static boost::thread_specific_ptr<StackTrace> m_LastStackTrace;
};

typedef boost::error_info<StackTrace, StackTrace> StackTraceErrorInfo;

class I2_BASE_API posix_error : virtual public std::exception, virtual public boost::exception { };

#ifdef _WIN32
class I2_BASE_API win32_error : virtual public std::exception, virtual public boost::exception { };

typedef boost::error_info<struct errinfo_win32_error_, int> errinfo_win32_error;

inline std::string to_string(const errinfo_win32_error& e)
{
	stringstream tmp;
	int code = e.value();

	char *message;
	String result = "Unknown error.";

	DWORD rc = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, 0, (char *)&message,
		0, NULL);

	if (rc != 0) {
		result = String(message);
		LocalFree(message);

		/* remove trailing new-line characters */
		boost::algorithm::trim_right(result);
	}

	tmp << code << ", \"" << result << "\"";
	return tmp.str();
}
#endif /* _WIN32 */

class I2_BASE_API openssl_error : virtual public std::exception, virtual public boost::exception { };

typedef boost::error_info<struct errinfo_openssl_error_, int> errinfo_openssl_error;

inline std::string to_string(const errinfo_openssl_error& e)
{
	stringstream tmp;
	int code = e.value();

	const char *message = ERR_error_string(code, NULL);

	if (message == NULL)
		message = "Unknown error.";

	tmp << code << ", \"" << message << "\"";
	return tmp.str();
}

}

#endif /* EXCEPTION_H */
