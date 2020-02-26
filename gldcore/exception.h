/* File: exception.h 
 * Copyright (C) 2008, Battelle Memorial Institute

	@file exception.h
	@addtogroup exception
	@ingroup core
@{
 **/

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>

#ifndef __cplusplus
/*	Define: TRY

	This macro implements a "try" block for C-style functions.

	This function is obsolete and should be replaced with a C++ "try" block.

	See Also: 
	- <THROW>
	- <CATCH>
	- <ENDCATCH>
 */
#define TRY DEPRECATED { EXCEPTIONHANDLER *_handler = create_exception_handler(); if (_handler==NULL) output_error("%s(%d): core exception handler creation failed",__FILE__,__LINE__); else if (setjmp(_handler->buf)==0) {
/* TROUBLESHOOT
	This error is caused when the system is unable to implement an exception handler for the core. 
	This is an internal error and should be reported to the core development team.
 */

/*	Define: THROW

	This macro implements a "throw" statement for C-style functions.

	This function is obsolete and should be replaced with a C++ "throw" block.

	See Also: 
	- <TRY>
	- <CATCH>
	- <ENDCATCH>
 */
#define THROW(...) DEPRECATED throw_exception(__VA_ARGS__);

/*	Define: CATCH

	This macro implements a "catch" statement for C-style functions.

	This function is obsolete and should be replaced with a C++ "catch" block.

	See Also: 
	- <TRY>
	- <THROW>
	- <ENDCATCH>
 */
#define CATCH(X) } DEPRECATED else {X = exception_msg();

/*	Define: ENDCATCH

	This macro ends a <CATCH> block.

	This function is obsolete and should be removed with the corresponding <CATCH> statement is replaced with a C++ "catch" block.

	See Also: 
	- <TRY>
	- <THROW>
	- <CATCH>
  */
#define ENDCATCH } DEPRECATED delete_exception_handler(_handler);}
#endif

DEPRECATED typedef struct s_exception_handler {
	int id; /**< the exception handler id */
	jmp_buf buf; /**< the \p jmpbuf containing the context for the exception handler */
	char msg[1024]; /**< the message thrown */
	struct s_exception_handler *next; /**< the next exception handler */
} EXCEPTIONHANDLER; /**< the exception handler structure */

#ifdef __cplusplus
extern "C" {
#endif

/*	Function: create_exception_handler

	This function is obsolete
*/
DEPRECATED EXCEPTIONHANDLER *create_exception_handler();

/*	Function: delete_exception_handler

	This function is obsolete
*/
DEPRECATED void delete_exception_handler(EXCEPTIONHANDLER *ptr);

/*	Function: throw_exception

	This function is obsolete
*/
DEPRECATED void throw_exception(const char *msg, ...);

/*	Function: exception_msg

	This function is obsolete
*/
DEPRECATED const char *exception_msg(void);

/*	Function: throwf

	This function is obsolete
*/
DEPRECATED void throwf(const char *format, ...);

#ifdef __cplusplus
}

#include <string>

/*	Class: GldException

	This class implement the general exception handler for GldMain.

	Example:
	--- C++ ---
	try 
	{
		if ( x < 0 )
			throw new GldException("failed: x = %f",x);
	}
	catch (GldException* exc)
	{
		cout << "Exception: " << exc->get_message();
	}
	---	
 */
class GldException 
{
private:
	std::string msg;
public:
	
	//	Constructor: GldException
	inline GldException(const char *format, ...)
	{
		va_list ptr;
		va_start(ptr,format);
		char *buf = NULL;
		try {
			if ( vasprintf(&buf,format,ptr) < 0 )
			{
				msg.assign("GldException::GldException(): vasprintf() failed");
			}
			else
			{
				msg.assign(buf);
			}
		}
		catch (...) 
		{
			msg.assign("GldException::GldException(): unknown exception in constructor");
		}
		if ( buf )
		{ 
			free(buf);
		}
		va_end(ptr);
	};

	//	Destructor: GldException()
	inline ~GldException(void)
	{
	};

	/*	Method: get_message

		This function retrieves the message string generated by the constructor.
	 */
	inline const char *get_message(void)
	{
		return msg.c_str();
	}

	/*	Method: throw_now

		This function is used to throw this exception object.
	 */
	inline void throw_now(void) { throw this;};
};

//	Class: GldAssert
class GldAssert
{
public:
	/*	Constructor: GldAssert
		Parameters:
		test - assertion test
		msg - message to use when throwing GldException on test failure
	 */
	inline GldAssert(bool test, const char *msg) 
	{ 
		if ( ! test ) 
			throw new GldException("GldAssert(): %s",msg); 
	};
	// Destructor: ~GldAssert
	inline ~GldAssert(void) {};
};

#endif

#endif

/**@}*/
