/*! 
@file epException.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
		<http://github.com/juhgiyo/eplibrary>
@date July 07, 2010
@brief Exception Interface
@version 2.0

@section LICENSE

Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section DESCRIPTION

An Interface for Exception.

*/
#ifndef __EP_EXCEPTION_H__
#define __EP_EXCEPTION_H__
#include "epLib.h"
#include <exception>

/*!
@def EP_VERIFY_EXCEPTION_W_MSG
@brief Macro Function for verify and throw exception with message.
@remark Expression must be 0 to throw the given exception like an Assert.
@param[in] _Expression the boolean expression to evaluate the exception
@param[in] _Exception the exception to throw
@param[in] _Message the message for the exception
*/
#define EP_VERIFY_EXCEPTION_W_MSG(_Expression,_Exception,_Message)\
	do{\
	if(!!!(_Expression)){\
	throw _Exception(_Message);\
	}\
	}while(0)

/*!
@def EP_VERIFY_EXCEPTION
@brief Macro Function for verify and throw exception.
@remark Expression must be 0 to throw the given exception like an Assert.
@param[in] _Expression the boolean expression to evaluate the exception
@param[in] _Exception the exception to throw
*/
#define EP_VERIFY_EXCEPTION(_Expression,_Exception)\
	do{\
	if(!!!(_Expression)){\
	throw _Exception();\
	}\
	}while(0)

	/// A type definition for ExceptionBadAlloc Type
	typedef std::bad_alloc ExceptionBadAlloc;

	/*!
	@def EP_VERIFY_BAD_ALLOC_W_MSG
	@brief Macro Function for verify and throw BadAlloc exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_BAD_ALLOC_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionBadAlloc,_Message)
	/*!
	@def EP_VERIFY_BAD_ALLOC
	@brief Macro Function for verify and throw BadAlloc exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_BAD_ALLOC(_Expression) EP_VERIFY_EXCEPTION(_Expression,ExceptionBadAlloc)


	/// A type definition for ExceptionLogicError Type
	typedef std::logic_error ExceptionLogicError;
	/*!
	@def EP_VERIFY_LOGIC_ERROR_W_MSG
	@brief Macro Function for verify and throw LogicError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_LOGIC_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionLogicError,_Message)
	/*!
	@def EP_VERIFY_LOGIC_ERROR
	@brief Macro Function for verify and throw LogicError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_LOGIC_ERROR(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionLogicError,"lock error")
	/// A type definition for ExceptionDomainError Type
	typedef std::domain_error ExceptionDomainError;
	/*!
	@def EP_VERIFY_DOMAIN_ERROR_W_MSG
	@brief Macro Function for verify and throw DomainError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_DOMAIN_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionDomainError,_Message)
	/*!
	@def EP_VERIFY_DOMAIN_ERROR
	@brief Macro Function for verify and throw DomainError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_DOMAIN_ERROR(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionDomainError,"domain error")
	/// A type definition for ExceptionInvalidArgument Type
	typedef std::invalid_argument ExceptionInvalidArgument;
	/*!
	@def EP_VERIFY_INVALID_ARGUMENT_W_MSG
	@brief Macro Function for verify and throw InvalidArgument exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_INVALID_ARGUMENT_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionInvalidArgument,_Message)
	/*!
	@def EP_VERIFY_INVALID_ARGUMENT
	@brief Macro Function for verify and throw InvalidArgument exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_INVALID_ARGUMENT(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionInvalidArgument,"invalid argument")
	/// A type definition for ExceptionLengthError Type
	typedef std::length_error ExceptionLengthError;
	/*!
	@def EP_VERIFY_LENGTH_ERROR_W_MSG
	@brief Macro Function for verify and throw LengthError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_LENGTH_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionLengthError,_Message)
	/*!
	@def EP_VERIFY_LENGTH_ERROR
	@brief Macro Function for verify and throw LengthError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_LENGTH_ERROR(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionLengthError,"length error")
	/// A type definition for ExceptionOutOfRange Type
	typedef std::out_of_range ExceptionOutOfRange;
	/*!
	@def EP_VERIFY_OUT_OF_RANGE_W_MSG
	@brief Macro Function for verify and throw OutOfRange exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_OUT_OF_RANGE_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionOutOfRange,_Message)
	/*!
	@def EP_VERIFY_OUT_OF_RANGE
	@brief Macro Function for verify and throw OutOfRange exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_OUT_OF_RANGE(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionOutOfRange,"out of range")
	
	/// A type definition for ExceptionRuntimeError Type
	typedef std::runtime_error ExceptionRuntimeError;
	/*!
	@def EP_VERIFY_RUNTIME_ERROR_W_MSG
	@brief Macro Function for verify and throw RuntimeError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_RUNTIME_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionRuntimeError,_Message)
	/*!
	@def EP_VERIFY_RUNTIME_ERROR
	@brief Macro Function for verify and throw RuntimeError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_RUNTIME_ERROR(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionRuntimeError,"runtime error")
	/// A type definition for ExceptionRangeError Type
	typedef std::range_error ExceptionRangeError;
	/*!
	@def EP_VERIFY_RANGE_ERROR_W_MSG
	@brief Macro Function for verify and throw RangeError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_RANGE_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionRangeError,_Message)
	/*!
	@def EP_VERIFY_RANGE_ERROR
	@brief Macro Function for verify and throw RangeError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_RANGE_ERROR(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionRangeError,"range error")
	/// A type definition for ExceptionOverflowError Type
	typedef std::overflow_error ExceptionOverflowError;
	/*!
	@def EP_VERIFY_OVERFLOW_ERROR_W_MSG
	@brief Macro Function for verify and throw OverflowError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_OVERFLOW_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionOverflowError,_Message)
	/*!
	@def EP_VERIFY_OVERFLOW_ERROR
	@brief Macro Function for verify and throw OverflowError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_OVERFLOW_ERROR(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionOverflowError,"overflow error")
	/// A type definition for ExceptionUnderflowError Type
	typedef std::underflow_error ExceptionUnderflowError;
	/*!
	@def EP_VERIFY_UNDERFLOW_ERROR_W_MSG
	@brief Macro Function for verify and throw UnderflowError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_UNDERFLOW_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionUnderflowError,_Message)
	/*!
	@def EP_VERIFY_UNDERFLOW_ERROR
	@brief Macro Function for verify and throw UnderflowError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_UNDERFLOW_ERROR(_Expression) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionUnderflowError,"underflow error")

	/// Thread Deadlock Error Static Message
	static const char * g_threadDeadLockErrorMessage=("thread deadlock error");

	/*! 
	@class ExceptionThreadDeadLockError epException.h
	@brief This is a class for ExceptionThreadDeadLockError Class

	Implements the ExceptionThreadDeadLockError Functions.
	*/
	class ExceptionThreadDeadLockError :public std::runtime_error
	{
	public:
		/*!
		Default Constructor

		Initializes the exception with given message
		@param[in] message exception message
		*/
		ExceptionThreadDeadLockError(const epl::EpString &message) throw():runtime_error(message)
		{
		}

		/*!
		Default Constructor

		Initializes the exception
		*/
		ExceptionThreadDeadLockError() throw()
			:runtime_error(g_threadDeadLockErrorMessage)
		{
		}

		/*!
		Default Destructor

		Destroy the exception
		*/
		virtual ~ExceptionThreadDeadLockError() throw()
		{}
#if !_HAS_EXCEPTIONS
	protected:
		/*!
		Raise the exception
		*/
		virtual void __CLR_OR_THIS_CALL _Doraise() const
		{	// perform class-specific exception handling
			_RAISE(*this);
		}
#endif// !_HAS_EXCEPTIONS
	};

/*!
	@def EP_VERIFY_THREAD_DEADLOCK_ERROR_W_MSG
	@brief Macro Function for verify and throw TreadDeadlockError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_THREAD_DEADLOCK_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionThreadDeadLockError,_Message)
/*!
	@def EP_VERIFY_THREAD_DEADLOCK_ERROR
	@brief Macro Function for verify and throw TreadDeadlockError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_THREAD_DEADLOCK_ERROR(_Expression) EP_VERIFY_EXCEPTION(_Expression,ExceptionThreadDeadLockError)


	/// Thread Creation Error Static Message
	static const char * g_threadCreationErrorMessage=("thread creation error");

	/*! 
	@class ExceptionThreadCreationError epException.h
	@brief This is a class for ExceptionThreadCreationError Class

	Implements the ExceptionThreadCreationError Functions.
	*/
	class ExceptionThreadCreationError :public std::runtime_error
	{
	public:
		/*!
		Default Constructor

		Initializes the exception with given message
		@param[in] message exception message
		*/
		ExceptionThreadCreationError(const epl::EpString &message) throw():runtime_error(message)
		{
		}

		/*!
		Default Constructor

		Initializes the exception
		*/
		ExceptionThreadCreationError() throw()
			:runtime_error(g_threadCreationErrorMessage)
		{
		}

		/*!
		Default Destructor

		Destroy the exception
		*/
		virtual ~ExceptionThreadCreationError() throw()
		{}
#if !_HAS_EXCEPTIONS
	protected:
		/*!
		Raise the exception
		*/
		virtual void __CLR_OR_THIS_CALL _Doraise() const
		{	// perform class-specific exception handling
			_RAISE(*this);
		}
#endif// !_HAS_EXCEPTIONS
	};


/*!
	@def EP_VERIFY_THREAD_CREATION_ERROR_W_MSG
	@brief Macro Function for verify and throw ThreadCreationError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_THREAD_CREATION_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionThreadCreationError,_Message)
/*!
	@def EP_VERIFY_THREAD_CREATION_ERROR
	@brief Macro Function for verify and throw ThreadCreationError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_THREAD_CREATION_ERROR(_Expression) EP_VERIFY_EXCEPTION(_Expression,ExceptionThreadCreationError)

 
	/// Thread Termination Error Static Message
	static const char * g_threadTerminationErrorMessage=("thread termination error");

	/*! 
	@class ExceptionThreadTerminationError epException.h
	@brief This is a class for ExceptionThreadTerminationError Class

	Implements the ExceptionThreadTerminationError Functions.
	*/
	class ExceptionThreadTerminationError :public std::runtime_error
	{
	public:
		/*!
		Default Constructor

		Initializes the exception with given message
		@param[in] message exception message
		*/
		ExceptionThreadTerminationError(const epl::EpString &message) throw():runtime_error(message)
		{
		}

		/*!
		Default Constructor

		Initializes the exception
		*/
		ExceptionThreadTerminationError() throw()
			:runtime_error(g_threadTerminationErrorMessage)
		{
		}

		/*!
		Default Destructor

		Destroy the exception
		*/
		virtual ~ExceptionThreadTerminationError() throw()
		{}
#if !_HAS_EXCEPTIONS
	protected:
		/*!
		Raise the exception
		*/
		virtual void __CLR_OR_THIS_CALL _Doraise() const
		{	// perform class-specific exception handling
			_RAISE(*this);
		}
#endif// !_HAS_EXCEPTIONS
	};

	/*!
	@def EP_VERIFY_THREAD_TERMINATION_ERROR_W_MSG
	@brief Macro Function for verify and throw TreadTerminationError exception with message.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	@param[in] _Message the message for the exception
	*/
#define EP_VERIFY_THREAD_TERMINATION_ERROR_W_MSG(_Expression,_Message) EP_VERIFY_EXCEPTION_W_MSG(_Expression,ExceptionThreadTerminationError,_Message)
/*!
	@def EP_VERIFY_THREAD_TERMINATION_ERROR
	@brief Macro Function for verify and throw TreadTerminationError exception.
	@remark Expression must be 0 to throw the exception like an Assert.
	@param[in] _Expression the boolean expression to evaluate the exception
	*/
#define EP_VERIFY_THREAD_TERMINATION_ERROR(_Expression) EP_VERIFY_EXCEPTION(_Expression,ExceptionThreadTerminationError)


#endif //__EP_EXCEPTION_H__