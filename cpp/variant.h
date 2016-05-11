#ifndef VARIENT_H_
#define VARIENT_H_

#define I_USE_ITERATOR

#include <string>
#include <algorithm>
#include <map>
#ifdef  I_USE_ITERATOR
#include <iterator>
#endif

#define in :


#define VAR_TYPE_NULL 0
#define VAR_TYPE_INT 1
#define VAR_TYPE_STRING 2
#define VAR_TYPE_MAP 3
#define VAR_TYPE_LIST 4
#define VAR_TYPE_DOUBLE 5
#define VAR_TYPE_FUNCTION 6
#define VAR_TYPE_NAN 7


class Array{
public:
	int m_array;
};

class null{
public:
	int m_null;
};

class NaN {
public:
	int m_nan;
};


class varIterator;

class var{
	friend varIterator;
public:
	typedef varIterator iterator;

	var::iterator begin();
	var::iterator end();

	var();
	var(int v);
	var(bool v);
	var(const char* str);
	var(const std::string& str);
	var(const var& val);
	var(double d);
	var(Array*);
	var(class null);
	var(void(*)(void));
	var(var(*)(void));
	var(void(*)(var a));
	var(var(*)(var a));
	var(class NaN n);

	void clear();
	void init();

	virtual ~var();

	var& operator[](const std::string& str);
	var& operator[](const char* str);
	var& operator[](int);
	var& operator[](const var&);

	var& at(int);

	var& operator =(const std::string& str);
	var& operator =(int val);
	var& operator =(unsigned int val);
	var& operator =(const var& val);
	var& operator =(double val);
	var& operator =(const char* val);
	var& operator =(bool val);
	var& operator =(Array*);
	var& operator =(class null);

	var& operator =(void(*)(void));
	var& operator =(var(*)(void));
	var& operator =(void(*)(var a));
	var& operator =(var(*)(var a));
	var& operator =(class NaN n);

	var operator()();
	var operator()(var a);

	bool operator ==(int);
	bool operator !=(int);

	bool operator ==(const var& val);
	bool operator !=(const var& val);

	bool operator ==(double);
	bool operator !=(double);

	bool operator ==(const char*);
	bool operator !=(const char*);

	bool operator ==(bool);
	bool operator !=(bool);

	bool operator ==(const std::string&);
	bool operator !=(const std::string&);

	bool operator ==(class null);
	bool operator !=(class null);


	var operator -();
	var operator +();
	var operator !();
	var operator ~();
	var& operator --();
	var& operator ++();
	var& operator --(int n);
	var& operator ++(int n);

	var& operator +=(const var& val);
	var& operator +=(const std::string& str);
	var& operator +=(const char* str);
	var& operator +=(int val);
	var& operator +=(double val);

	var operator +(const var& val);
	var operator +(const std::string& str);
	var operator +(const char* str);
	var operator +(int val);
	var operator +(unsigned int val);
	var operator +(double val);

	var operator -(const var& val);
	var operator -(int val);
	var operator -(unsigned int val);
	var operator -(double val);

	var operator *(const var& val);
	var operator *(int val);
	var operator *(double val);

	var operator /(const var& val);
	var operator /(int val);
	var operator /(double val);

	var operator %(const var& val);
	var operator %(int val);

	var operator <<(const var& val);
	var operator <<(int val);

	var operator >>(const var& val);
	var operator >>(int val);

	var operator &(const var& val);
	var operator &(int val);

	var operator |(const var& val);
	var operator |(int val);

	var operator ^(const var& val);
	var operator ^(int val);


	bool operator &&(const var& val);
	bool operator &&(int val);
	bool operator &&(double val);

	bool operator ||(const var& val);
	bool operator ||(int val);
	bool operator ||(double val);

	bool operator >(const var& val);
	bool operator >(int val);
	bool operator >(double val);

	bool operator >=(int val);
	bool operator >=(double val);
	bool operator >=(const var& val);

	bool operator <(const var& val);
	bool operator <(int val);
	bool operator <(double val);

	bool operator <=(const var& val);
	bool operator <=(int val);
	bool operator <=(double val);

	
	var& operator -=(const var& val);
	var& operator -=(int val);
	var& operator -=(double val);

	
	var& operator *=(const var& val);
	var& operator *=(int val);
	var& operator *=(double val);

	
	var& operator /=(const var& val);
	var& operator /=(int val);
	var& operator /=(double val);

	var& operator %=(const var& val);
	var& operator %=(int val);

	var& operator &=(const var& val);
	var& operator &=(int val);

	var& operator |=(const var& val);
	var& operator |=(int val);

	var& operator <<=(const var& val);
	var& operator <<=(int val);

	var& operator >>=(const var& val);
	var& operator >>=(int val);






	operator std::string();
	operator int() const;
	operator unsigned int() const;
	operator double() const;
	operator float() const;

	int toInt();
	double toFloat();
	std::string toString();

	int length();
	const char* pchar();

	//string
	std::string charAt(int);
	int charCodeAt(int);
	int indexOf(std::string a);
	int lastIndexOf(std::string a);
	std::string toLowerCase();
	std::string toUpperCase();
	std::string trim();
	std::string replace(std::string a,std::string b);
	std::string substr(int a);
	std::string substr(int a,int l);
	std::string substring(int a);
	std::string substring(int a,int l);

	//map
	void delete_(std::string a);


private:
	//void tumeru();

public:

	int type;

	std::map<std::string,var*> m_map;
	std::string m_str;
	int m_val;
	float m_fval;

	var(*m_func1)(void);
	void(*m_func2)(void);
	var(*m_func3)(var a);
	void(*m_func4)(var a);

	int m_max_array;
	bool m_is_array;
};

var typeof(int);
var typeof(double);
var typeof(var&);
var typeof(char*);

class String{
public:
	static std::string fromCharCode(int a);
	static std::string fromCharCode(int a,int b);
	static std::string fromCharCode(int a,int b,int c);
	static std::string fromCharCode(int a,int b,int c,int d);
};

var parseInt(var val);
var parseFloat(var val);

class sessionStorage{
public :
	static std::string getItem(std::string key);
	static void setItem(std::string key, std::string val);
	static void removeItem(std::string key);
};

class localStorage{
public:
	static std::string getItem(std::string key);
	static void setItem(std::string key, std::string val);
	static void removeItem(std::string key);
};

class Math{
public:
	static int floor(var a);
	static int round(var a);
	static int trunc(var a);
	static double abs(var a);

	static double sin(var a);
	static double cos(var a);
	static double tan(var a);
	static double random();

	static double exp(var a);
	static double pow(var a,var b);
	static double sqrt(var a);
	static double log(var a);
	static double log10(var a);

};


std::string encodeURIComponent(std::string str);
std::string decodeURIComponent(std::string str);

var typeof(sessionStorage& a);
var typeof(localStorage& a);

bool isNaN(var a);


static String String;
static null null;
static sessionStorage sessionStorage;
static localStorage localStorage;
static NaN NaN;
static Math Math;



#ifdef  I_USE_ITERATOR

class varIterator : public std::iterator<std::forward_iterator_tag, var>
{
	friend var;
private:
private:
	varIterator();
	varIterator(var* myClass, int index);

public:
	varIterator(const varIterator& iterator);

public:
	varIterator& operator++();
	varIterator operator++(int);

	var operator*();

	bool operator==(const varIterator& iterator);
	bool operator!=(const varIterator& iterator);

private:
	int m_index;
	var* m_var;

};
#endif



#ifdef  VARIENT_DEBUG_TEST
void println(const char*);
void println(var );
#endif


#endif 



