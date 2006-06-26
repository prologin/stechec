#ifndef INVALIDPARAMETEREXCEPTION_HH_
#define INVALIDPARAMETEREXCEPTION_HH_

class InvalidParameterException
{
public:
    const char* msg; 

    InvalidParameterException(const char* m);
	virtual ~InvalidParameterException();
};

inline InvalidParameterException::InvalidParameterException(const char* m) { msg = m; }

inline InvalidParameterException::~InvalidParameterException() { }

#endif /*INVALIDPARAMETEREXCEPTION_HH_*/
