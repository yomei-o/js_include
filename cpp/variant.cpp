//#define  VARIENT_DEBUG_TEST

#ifndef JAVASCRIPT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "variant.h"

#if defined(_WIN32) && !defined(__GNUC__)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif


#endif


#ifdef JAVASCRIPT 
#define length() length
#define size() size
#endif

#ifdef JAVASCRIPT
#define clone(a) function(src){var dst = {};for(var k in src)dst[k] = src[k];return dst;}(a)
#else
var clone(var& a)
{
	return a;
}
#endif

#ifdef JAVASCRIPT 
#define _delete(a,b) delete a[b]
#else
#define _delete(a,b) a.delete_(b)
#endif

//
// function and global value
//

#ifdef VARIENT_DEBUG_TEST

#ifdef JAVASCRIPT
println=function(src)
#else
void println(var src)
#endif
{
#ifdef JAVASCRIPT
	alert(src);
#else
	printf("%s\n",((std::string)src).c_str());
#endif
}

#ifndef JAVASCRIPT
void println(const char* a)
{
	var aa;
	aa=a;
	println(aa);
}
#endif

#endif



//
// variant
//

#ifndef JAVASCRIPT


static var var_null;


void var::init()
{
	type=0;
	m_val=0;
	m_fval=0;

	m_max_array=0;
	m_is_array=false;

	m_func1 = NULL;
	m_func2 = NULL;
	m_func3 = NULL;
	m_func4 = NULL;
}
var::var()
{
	init();
}


var::var(int v)
{
	init();
	type=VAR_TYPE_INT;
	m_val=v;
	m_fval=0;
}

var::var(bool v)
{
	init();
	type=VAR_TYPE_INT;
	m_val=(int)v;
	m_fval=0;
}


var::var(const char*str)
{
	init();
	if(str==NULL)return;
	type=VAR_TYPE_STRING;
	m_str=str;
	m_fval=0;

}

var::var(const std::string& str)
{
	init();
	type=VAR_TYPE_STRING;
	m_str=str;
	m_fval=0;
}


var::var(const var& val)
{
	init();
	(*this)=val;
}


var::var(double v)
{
	init();
	type=VAR_TYPE_DOUBLE;
	m_val=0;
	m_fval=(float)v;
}

var::var(Array* o)
{
	init();
	if(o)delete o;
	type=VAR_TYPE_MAP;
	m_is_array=true;
}

var::var(class null o)
{
	init();
}



var::~var()
{
	clear();
}

void var::clear()
{
	var* ret;
	std::map<std::string,var*>::iterator ite;

	ite=m_map.begin();
	while( ite != m_map.end() ) 
	{
		ret=(*ite).second;
		delete ret;
		ite++;
	}
	type=0;
	m_val=0;
	m_str.clear();
	m_map.clear();
	
	//m_min_array=0;
	m_max_array=0;
	m_is_array=false;
}


var& var::operator[](const char* str)
{
	if(str==NULL){
		return var_null;
	}
	return operator[]((std::string)str);
}

var& var::operator[](const std::string& str)
{
	var* ret=&var_null;
	
	if(type!=0 && type!=VAR_TYPE_MAP){
		return *ret;
	}

	switch(type){
	case 0:
		type=VAR_TYPE_MAP;
		m_is_array=false;
	case VAR_TYPE_MAP:
		m_is_array=false;
		ret=m_map[str];
		if(ret==NULL){
			ret=new var();
			m_map[str]=ret;
		}
		break;
	}
	return *ret;
}

var& var::operator[](int v)
{
	var* ret=&var_null;
	char tmp[64];

	tmp[0]=0;
	sprintf(tmp,"%d",v);
	std::string str=tmp;

	if(type!=0 && type!=VAR_TYPE_MAP){
		return *ret;
	}

	switch(type){
	case 0:
		type=VAR_TYPE_MAP;
		m_is_array=true;
	case VAR_TYPE_MAP:
		ret=m_map[str];
		if(ret==NULL){
			ret=new var();
			m_map[str]=ret;
		}
		if(m_is_array){
			if(v<0)m_is_array=false;
			if(m_max_array<v+1)m_max_array=v+1;
		}
		break;
	}
	return *ret;
}

var& var::operator[](const var& v)
{
	var* ret=&var_null;
	char tmp[64];

	tmp[0]=0;
	sprintf(tmp,"%d",(int)v);
	std::string str=tmp;

	if(type!=0 && type!=VAR_TYPE_MAP){
		return *ret;
	}
	if(v.type!=VAR_TYPE_INT && v.type!=VAR_TYPE_DOUBLE && v.type!=VAR_TYPE_STRING){
		return *ret;
	}
	if (v.type == VAR_TYPE_STRING) {
		var t;
		t = v;
		str = (std::string)t;
	}

	switch(type){
	case 0:
		type=VAR_TYPE_MAP;
		m_is_array=true;
	case VAR_TYPE_MAP:
		ret=m_map[str];
		if(ret==NULL){
			ret=new var();
			m_map[str]=ret;
		}
		if(v.type==VAR_TYPE_INT){
			if(m_is_array){
				if(v.m_val<0)m_is_array=false;
				if(m_max_array<v.m_val+1)m_max_array=v.m_val+1;
			}
		}
		if(v.type==VAR_TYPE_DOUBLE){
			if(m_is_array){
				if((int)(v.m_fval)<0)m_is_array=false;
				if(m_max_array<(int)(v.m_fval)+1)m_max_array=(int)(v.m_fval)+1;
			}
		}
		if (v.type == VAR_TYPE_STRING) {
			m_is_array = false;
			ret = m_map[str];
			if (ret == NULL) {
				ret = new var();
				m_map[str] = ret;
			}
		}
		break;
	}
	return *ret;
}


var& var::at(int n)
{
	if(type!=VAR_TYPE_MAP || n<0){
		return var_null;
	}
	std::map<std::string,var*>::iterator ite;
	int ct=0;

	ite=m_map.begin();
	while( ite != m_map.end() ) 
	{
		if(ct==n)return *(*ite).second;
		ite++;
		ct++;
	}

	return var_null;
}


var& var::operator =(const std::string& str)
{
	clear();

	type=VAR_TYPE_STRING;
	m_str=str;
	return *this;
}

var& var::operator =(const char* str)
{
	clear();
	if(str==NULL){
		type=0;
		return *this;
	}
	type=VAR_TYPE_STRING;
	m_str=str;
	return *this;
}

var& var::operator =(int val)
{
	clear();

	type=VAR_TYPE_INT;
	m_val=val;

	return *this;
}

var& var::operator =(unsigned int val)
{
	clear();

	type = VAR_TYPE_INT;
	m_val = val;

	return *this;
}


var& var::operator =(bool val)
{
	clear();

	type=VAR_TYPE_INT;
	m_val=(int)val;

	return *this;
}


var& var::operator =(double val)
{
	clear();

	type=VAR_TYPE_DOUBLE;
	m_fval=(float)val;

	return *this;
}



var& var::operator =(const var& val)
{
	clear();

	switch(val.type){
	case VAR_TYPE_NULL:
		break;
	case VAR_TYPE_INT:
		type=VAR_TYPE_INT;
		m_val=val.m_val;
		break;
	case VAR_TYPE_STRING:
		type=VAR_TYPE_STRING;
		m_str=val.m_str;
		break;
	case VAR_TYPE_DOUBLE:
		type=VAR_TYPE_DOUBLE;
		m_fval=val.m_fval;
		break;
	case VAR_TYPE_FUNCTION:
		type = VAR_TYPE_FUNCTION;
		m_func1 = val.m_func1;
		m_func2 = val.m_func2;
		m_func3 = val.m_func3;
		m_func4 = val.m_func4;
		break;
	case VAR_TYPE_NAN:
		type = VAR_TYPE_NAN;
		break;
	case VAR_TYPE_MAP:
		std::map<std::string,var*>::iterator ite;
		std::map<std::string,var*>& a=(std::map<std::string,var*>&)(val.m_map);
		type=VAR_TYPE_MAP;
		ite=a.begin();

		while( ite != val.m_map.end() ) 
		{
			m_map[(*ite).first]=new var();
			*(m_map[(*ite).first])=*(*ite).second;
			ite++;
		}
		break;
	}
	return *this;
}

var& var::operator =(Array* o)
{
	clear();
	if(o)delete o;
	type=VAR_TYPE_MAP;
	m_is_array=true;
	return *this;
}

var& var::operator =(class null o)
{
	clear();
	return *this;
}


var::operator std::string()
{
	switch(type){
	case 0:
		return "null";
	case VAR_TYPE_STRING:
		return m_str;
	case VAR_TYPE_INT:
	case VAR_TYPE_DOUBLE:
		return toString();
	case VAR_TYPE_MAP:
			return "object";
	}
	return "broken";
}

var::operator int() const
{
	switch(type){
	case 0:
		return 0;
	case VAR_TYPE_INT:
		return m_val;
	case VAR_TYPE_DOUBLE:
		return (int)m_fval;
	case VAR_TYPE_STRING:
		return ((var*)this)->toInt();
	}
	return -1;
}

var::operator unsigned int() const
{
	switch (type){
	case 0:
		return 0;
	case VAR_TYPE_INT:
		return m_val;
	case VAR_TYPE_DOUBLE:
		return (int)m_fval;
	case VAR_TYPE_STRING:
		return ((var*)this)->toInt();
	}
	return -1;
}

var::operator float() const
{
	switch(type){
	case 0:
		return 0;
	case VAR_TYPE_INT:
		return (float)m_val;
	case VAR_TYPE_DOUBLE:
		return m_fval;
	case VAR_TYPE_STRING:
		return (float)(((var*)this)->toFloat());
	}
	return -1;
}

var::operator double() const
{
	switch(type){
	case 0:
		return 0;
	case VAR_TYPE_INT:
		return (double)m_val;
	case VAR_TYPE_DOUBLE:
		return (double)m_fval;
	case VAR_TYPE_STRING:
		return ((var*)this)->toFloat();
	}
	return -1;
}

const char* var::pchar()
{
	switch(type){
	case 0:
		return "null";
	case VAR_TYPE_STRING:
		return m_str.c_str();
	case VAR_TYPE_INT:
		return toString().c_str();
	case VAR_TYPE_MAP:
		return "objrct";
	}
	return "broken";
}

int var::toInt()
{
	int ret=0;
	switch(type){
	case 0:
		return 0;
	case VAR_TYPE_INT:
		return m_val;
	case VAR_TYPE_DOUBLE:
		return (int)m_fval;
	case VAR_TYPE_STRING:
		ret=-1;
		sscanf(m_str.c_str(),"%d",&ret);
		return ret;
	}
	return ret;
}

double var::toFloat()
{
	double ret=0;
	switch(type){
	case 0:
		return 0;
	case VAR_TYPE_INT:
		return (double)m_val;
	case VAR_TYPE_DOUBLE:
		return m_fval;
	case VAR_TYPE_STRING:
		ret=-1;
		sscanf(m_str.c_str(),"%lf",&ret);
		return ret;
	}
	return ret;
}

std::string var::toString()
{
	std::string ret;
	char tmp[64];
	switch(type){
	case 0:
		return "0";
	case VAR_TYPE_INT:
		tmp[0]=0;
		sprintf(tmp,"%d",m_val);
		ret=tmp;
		return ret;
	case VAR_TYPE_DOUBLE:
		tmp[0]=0;
		sprintf(tmp,"%f",m_fval);
		ret=tmp;
		return ret;
	case VAR_TYPE_STRING:
		return m_str;
	}
	return ret;
}


bool var::operator ==(int v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return false;
	}
	return operator ==(w);
}

bool var::operator !=(int v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return true;
	}
	return operator !=(w);
}

bool var::operator ==(bool v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return false;
	}
	return operator ==(w);
}

bool var::operator !=(bool v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return true;
	}
	return operator !=(w);
}


bool var::operator ==(double v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return false;
	}
	return operator ==(w);
}

bool var::operator !=(double v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return true;
	}
	return operator !=(w);
}


bool var::operator ==(const char* v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return false;
	}
	return operator ==(w);
}

bool var::operator !=(const char* v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return true;
	}
	return operator !=(w);
}



bool var::operator ==(const std::string& v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return false;
	}
	return operator ==(w);
}

bool var::operator !=(const std::string& v)
{
	var w;
	w=v;
	switch(type){
	case 0:
		return true;
	}
	return operator !=(w);
}




bool var::operator ==(const var& val)
{
	switch(type){
	case 0:
		if(val.type==VAR_TYPE_NULL)return true;
		return false;
	case VAR_TYPE_INT:
		if(val.type==VAR_TYPE_INT && m_val==val.m_val)return true;
		if(val.type==VAR_TYPE_STRING && m_val==(int)atoi(val.m_str.data()))return true;
		if(val.type==VAR_TYPE_DOUBLE && m_val==val.m_fval)return true;
		return false;
	case VAR_TYPE_DOUBLE:
		if(val.type==VAR_TYPE_DOUBLE && m_fval==val.m_fval)return true;
		if(val.type==VAR_TYPE_INT && m_fval==(float)(val.m_val))return true;
		if(val.type==VAR_TYPE_STRING && m_fval==(float)atof(val.m_str.data()))return true;
		return false;
	case VAR_TYPE_STRING:
		if(m_str=="" && val.type==VAR_TYPE_INT&& val.m_val==0)return true;
		if(m_str=="" && val.type==VAR_TYPE_DOUBLE && val.m_fval==0)return true;
		if(val.type==VAR_TYPE_INT || VAR_TYPE_DOUBLE){
			var tmp;
			tmp=val;
			if(m_str==tmp.toString())return true;
		}
		if(val.type==VAR_TYPE_STRING && m_str==val.m_str)return true;
		return false;
	case VAR_TYPE_FUNCTION:
		if (val.type != VAR_TYPE_FUNCTION)return false;
		if (m_func1 == val.m_func1 && m_func2 == val.m_func2 &&m_func3 == val.m_func3 &&m_func4 == val.m_func4)return true;
		return false;
	case VAR_TYPE_MAP:
		//no implement
		return false;
	}
	return false;
}

bool var::operator !=(const var& val)
{
	switch(type){
	case 0:
		if(val.type==VAR_TYPE_NULL)return false;
		return true;
	case VAR_TYPE_INT:
		if(val.type==VAR_TYPE_INT && m_val==val.m_val)return false;
		if(val.type==VAR_TYPE_STRING && m_val==(int)atoi(val.m_str.data()))return false;
		if(val.type==VAR_TYPE_DOUBLE && m_val==val.m_fval)return false;
		return true;
	case VAR_TYPE_DOUBLE:
		if(val.type==VAR_TYPE_DOUBLE && m_fval==val.m_fval)return false;
		if(val.type==VAR_TYPE_INT && m_fval==(float)(val.m_val))return false;
		if(val.type==VAR_TYPE_STRING && m_fval==(float)atof(val.m_str.data()))return false;
		return true;
	case VAR_TYPE_STRING:
		if(m_str=="" && val.type==VAR_TYPE_INT&& val.m_val==0)return false;
		if(m_str=="" && val.type==VAR_TYPE_DOUBLE && val.m_fval==0)return false;
		if(val.type==VAR_TYPE_INT || VAR_TYPE_DOUBLE){
			var tmp;
			tmp=val;
			if(m_str==tmp.toString())return false;
		}
		if(val.type==VAR_TYPE_STRING && m_str==val.m_str)return false;
		return true;
	case VAR_TYPE_FUNCTION:
		if (val.type != VAR_TYPE_FUNCTION)return true;
		if (m_func1 == val.m_func1 && m_func2 == val.m_func2 &&m_func3 == val.m_func3 &&m_func4 == val.m_func4)return false;
		return true;
	case VAR_TYPE_MAP:
		//no implement
		return true;
	}
	return true;
}

bool var::operator ==(class null val)
{
	if(type==VAR_TYPE_NULL)return true;
	return false;
}

bool var::operator !=(class null val)
{
	if(type==VAR_TYPE_NULL)return false;
	return true;
}


int var::length()
{
	int ret=0;
	switch(type){
	case 0:
		return 0;
	case VAR_TYPE_STRING:
		return m_str.length();
	case VAR_TYPE_INT:
		if(m_val>=0)return m_val;
		return -m_val;
	case VAR_TYPE_MAP:
		if(m_is_array){
			return m_max_array;
		}
		ret=m_map.size();
		return ret;
	}
	return ret;
}

//
//
//
//
//
//


var& var::operator +=(const var& val)
{
	char tmp[128];

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			type=VAR_TYPE_DOUBLE;
			m_fval=val.m_fval;
			break;
		case VAR_TYPE_STRING:
			type=VAR_TYPE_STRING;
			m_str=val.m_str;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			break;
		case VAR_TYPE_INT:
			m_val+=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			type=VAR_TYPE_DOUBLE;
			m_fval=(float)m_val+val.m_fval;
			break;
		case VAR_TYPE_STRING:
			type=VAR_TYPE_STRING;
			sprintf(tmp,"%d",m_val);
			m_str=tmp;
			m_str.append(val.m_str);
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			break;
		case VAR_TYPE_INT:
			m_fval+=(float)(val.m_val);
			break;
		case VAR_TYPE_DOUBLE:
			m_fval+=val.m_fval;
			break;
		case VAR_TYPE_STRING:
			type=VAR_TYPE_STRING;
			sprintf(tmp,"%g",m_fval);
			m_str=tmp;
			m_str.append(val.m_str);
			break;
		}
		break;
	case VAR_TYPE_STRING:
		switch(val.type){
		case 0:
			break;
		case VAR_TYPE_INT:
			sprintf(tmp,"%d",val.m_val);
			m_str.append(tmp);
			break;
		case VAR_TYPE_DOUBLE:
			sprintf(tmp,"%g",val.m_fval);
			m_str.append(tmp);
			break;
		case VAR_TYPE_STRING:
			m_str.append(val.m_str);
			break;
		}
		break;
	}
	return *this;
}

var& var::operator +=(const std::string& v)
{
	var a;
	a=v;

	return operator +=(a);
}
var& var::operator +=(const char *v)
{
	var a;
	a=v;

	return operator +=(a);
}

var& var::operator +=(int v)
{
	var a;
	a=v;

	return operator +=(a);
}

var& var::operator +=(double v)
{
	var a;
	a=v;

	return operator +=(a);
}

//
//
//
var var::operator +(const var& val)
{
	char tmp[128];
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=val.m_fval;
			break;
		case VAR_TYPE_STRING:
			ret.type=VAR_TYPE_STRING;
			ret.m_str=val.m_str;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val+val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=(float)m_val+val.m_fval;
			break;
		case VAR_TYPE_STRING:
			ret.type=VAR_TYPE_STRING;
			sprintf(tmp,"%d",m_val);
			ret.m_str=tmp;
			ret.m_str.append(val.m_str);
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval+(float)(val.m_val);
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval+val.m_fval;
			break;
		case VAR_TYPE_STRING:
			ret.type=VAR_TYPE_STRING;
			sprintf(tmp,"%g",m_fval);
			ret.m_str=tmp;
			ret.m_str.append(val.m_str);
			break;
		}
		break;
	case VAR_TYPE_STRING:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_STRING;
			ret.m_str=m_str;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_STRING;
			sprintf(tmp,"%d",val.m_val);
			ret.m_str=m_str;
			ret.m_str.append(tmp);
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_STRING;
			sprintf(tmp,"%g",val.m_fval);
			ret.m_str=m_str;
			ret.m_str.append(tmp);
			break;
		case VAR_TYPE_STRING:
			ret.type=VAR_TYPE_STRING;
			ret.m_str=m_str;
			ret.m_str.append(val.m_str);
			break;
		}
		break;
	}
	return ret;
}

var var::operator +(const char* v)
{
	var w;
	if(v)w=v;
	return operator +(w);
}

var var::operator +(const std::string& v)
{
	var w;
	w=v;
	return operator +(w);
}

var var::operator +(int v)
{
	var w;
	w=v;
	return operator +(w);
}

var var::operator +(unsigned int v)
{
	var w;
	w = v;
	return operator +(w);
}

var var::operator +(double v)
{
	var w;
	w=v;
	return operator +(w);
}



//
// tankou enzansi 
//

var var::operator -()
{
	var ret;
	switch(type){
	case 0:
		ret.type=VAR_TYPE_INT;
		ret.m_val=-m_val;
		break;
	case VAR_TYPE_INT:
		ret.type=VAR_TYPE_INT;
		ret.m_val=-m_val;
		break;
	case VAR_TYPE_DOUBLE:
		ret.type=VAR_TYPE_DOUBLE;
		ret.m_fval=-m_fval;
		break;
	}
	return ret;
}

var var::operator +()
{
	var ret;
	switch(type){
	case 0:
		ret.type=VAR_TYPE_INT;
		ret.m_val=m_val;
		break;
	case VAR_TYPE_INT:
		ret.type=VAR_TYPE_INT;
		ret.m_val=m_val;
		break;
	case VAR_TYPE_DOUBLE:
		ret.type=VAR_TYPE_DOUBLE;
		ret.m_fval=m_fval;
		break;
	}
	return ret;
}

var var::operator !()
{
	var ret;
	switch(type){
	case 0:
		ret.type=VAR_TYPE_INT;
		ret.m_val=m_val;
		break;
	case VAR_TYPE_INT:
		ret.type=VAR_TYPE_INT;
		if(m_val)ret.m_val=1;
		break;
	case VAR_TYPE_DOUBLE:
		ret.type=VAR_TYPE_INT;
		if(m_fval!=0.0)ret.m_val=1;
		break;
	}
	return ret;
}

var var::operator ~()
{
	var ret;
	switch(type){
	case 0:
		ret.type=VAR_TYPE_INT;
		ret.m_val=~m_val;
		break;
	case VAR_TYPE_INT:
		ret.type=VAR_TYPE_INT;
		ret.m_val=~m_val;
		break;
	case VAR_TYPE_DOUBLE:
		ret.type=VAR_TYPE_INT;
		ret.m_val=~((int)m_fval);
		break;
	}
	return ret;
}


//
// ++ --
//
var& var::operator ++()
{
	switch(type){
	case 0:
		type=VAR_TYPE_INT;
		m_val++;
		break;
	case VAR_TYPE_INT:
		m_val++;
		break;
	case VAR_TYPE_DOUBLE:
		m_fval++;
		break;
	}
	return *this;
}

var& var::operator ++(int n)
{
	switch(type){
	case 0:
		type=VAR_TYPE_INT;
		m_val++;
		break;
	case VAR_TYPE_INT:
		m_val++;
		break;
	case VAR_TYPE_DOUBLE:
		m_fval++;
		break;
	}
	return *this;
}

var& var::operator --()
{
	switch(type){
	case 0:
		type=VAR_TYPE_INT;
		m_val--;
		break;
	case VAR_TYPE_INT:
		m_val--;
		break;
	case VAR_TYPE_DOUBLE:
		m_fval--;
		break;
	}
	return *this;
}

var& var::operator --(int n)
{
	switch(type){
	case 0:
		type=VAR_TYPE_INT;
		m_val--;
		break;
	case VAR_TYPE_INT:
		m_val--;
		break;
	case VAR_TYPE_DOUBLE:
		m_fval--;
		break;
	}
	return *this;
}


//
// operator -
//
var var::operator -(const var& val)
{
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=-val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=-val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=-m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val-val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=(float)m_val-val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=-m_fval;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval-(float)(val.m_val);
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval-val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

var var::operator - (int v)
{
	var w;
	w=v;
	return operator -(w);
}

var var::operator - (unsigned int v)
{
	var w;
	w = v;
	return operator -(w);
}

var var::operator -(double v)
{
	var w;
	w=v;
	return operator -(w);
}


//
// operator *
//
var var::operator *(const var& val)
{
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val*val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val*val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=(float)m_val*val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val*val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val*val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=(float)m_val*val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval*(float)(val.m_val);
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval*(float)(val.m_val);
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval*val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

var var::operator * (int v)
{
	var w;
	w=v;
	return operator *(w);
}

var var::operator *(double v)
{
	var w;
	w=v;
	return operator *(w);
}

//
// operator /
//
var var::operator /(const var& val)
{
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val/val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val/val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=(float)m_val/val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val/val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val/val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=(float)m_val/val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval/(float)(val.m_val);
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval/(float)(val.m_val);
			break;
		case VAR_TYPE_DOUBLE:
			ret.type=VAR_TYPE_DOUBLE;
			ret.m_fval=m_fval/val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

var var::operator / (int v)
{
	var w;
	w=v;
	return operator /(w);
}

var var::operator /(double v)
{
	var w;
	w=v;
	return operator /(w);
}

//
// operator %
//
var var::operator %(const var& val)
{
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val%val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val%val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val%val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val%val.m_val;
			break;
		}
		break;
	}
	return ret;
}

var var::operator % (int v)
{
	var w;
	w=v;
	return operator %(w);
}


//
// operator <<
//
var var::operator <<(const var& val)
{
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val<<val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val<<val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val<<val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val<<val.m_val;
			break;
		}
		break;
	}
	return ret;
}

var var::operator << (int v)
{
	var w;
	w=v;
	return operator <<(w);
}


//
// operator >>
//
var var::operator >>(const var& val)
{
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val>>val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val>>val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val>>val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val>>val.m_val;
			break;
		}
		break;
	}
	return ret;
}

var var::operator >> (int v)
{
	var w;
	w=v;
	return operator >>(w);
}


//
// operator &
//
var var::operator &(const var& val)
{
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val&val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val&val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val&val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val&val.m_val;
			break;
		}
		break;
	}
	return ret;
}

var var::operator & (int v)
{
	var w;
	w=v;
	return operator &(w);
}


//
// operator ^
//
var var::operator ^(const var& val)
{
	var ret;

	switch (type){
	case 0:
		switch (val.type){
		case 0:
			ret.type = VAR_TYPE_INT;
			ret.m_val = m_val^val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type = VAR_TYPE_INT;
			ret.m_val = m_val^val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch (val.type){
		case 0:
			ret.type = VAR_TYPE_INT;
			ret.m_val = m_val^val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type = VAR_TYPE_INT;
			ret.m_val = m_val^val.m_val;
			break;
		}
		break;
	}
	return ret;
}

var var::operator ^ (int v)
{
	var w;
	w = v;
	return operator ^(w);
}



//
// operator |
//
var var::operator |(const var& val)
{
	var ret;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val|val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val|val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val|val.m_val;
			break;
		case VAR_TYPE_INT:
			ret.type=VAR_TYPE_INT;
			ret.m_val=m_val|val.m_val;
			break;
		}
		break;
	}
	return ret;
}

var var::operator | (int v)
{
	var w;
	w=v;
	return operator |(w);
}

//
// operator &&
//
bool var::operator &&(const var& val)
{
	bool ret=false;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret=m_val && val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val && val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val && val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret=m_val && val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val && val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val && val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret=m_fval && val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_fval && val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_fval && val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

bool var::operator && (int v)
{
	var w;
	w=v;
	return operator &&(w);
}

bool var::operator &&(double v)
{
	var w;
	w=v;
	return operator &&(w);
}


//
// operator ||
//
bool var::operator ||(const var& val)
{
	bool ret=false;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret=m_val || val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val || val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val || val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret=m_val || val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val || val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val || val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret=m_fval || val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_fval || val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_fval || val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

bool var::operator || (int v)
{
	var w;
	w=v;
	return operator ||(w);
}

bool var::operator ||(double v)
{
	var w;
	w=v;
	return operator ||(w);
}


//
// operator >
//
bool var::operator >(const var& val)
{
	bool ret=false;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret=m_val > val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val > val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val > val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret=m_val > val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val > val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val > val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret=m_fval > val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_fval > val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_fval > val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

bool var::operator > (int v)
{
	var w;
	w=v;
	return operator > (w);
}

bool var::operator >(double v)
{
	var w;
	w=v;
	return operator >(w);
}

//
// operator >=
//
bool var::operator >=(const var& val)
{
	bool ret=false;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret=m_val >= val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val >= val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val >= val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret=m_val >= val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val >= val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val >= val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret=m_fval >= val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_fval >= val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_fval >= val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

bool var::operator >= (int v)
{
	var w;
	w=v;
	return operator >= (w);
}

bool var::operator >=(double v)
{
	var w;
	w=v;
	return operator >=(w);
}

//
// operator <
//
bool var::operator <(const var& val)
{
	bool ret=false;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret=m_val < val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val < val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val < val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret=m_val < val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val < val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val < val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret=m_fval < val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_fval < val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_fval < val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

bool var::operator < (int v)
{
	var w;
	w=v;
	return operator < (w);
}

bool var::operator <(double v)
{
	var w;
	w=v;
	return operator <(w);
}


//
// operator <=
//
bool var::operator <=(const var& val)
{
	bool ret=false;

	switch(type){
	case 0:
		switch(val.type){
		case 0:
			ret=m_val <= val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val <= val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val <= val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			ret=m_val <= val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_val <= val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_val <= val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			ret=m_fval <= val.m_val;
			break;
		case VAR_TYPE_INT:
			ret=m_fval <= val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			ret= m_fval <= val.m_fval;
			break;
		}
		break;
	}
	return ret;
}

bool var::operator <= (int v)
{
	var w;
	w=v;
	return operator <= (w);
}

bool var::operator <=(double v)
{
	var w;
	w=v;
	return operator <=(w);
}




//
// operator -=
//


var& var::operator -=(const var& val)
{
	switch(type){
	case 0:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val-=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val-=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			type=VAR_TYPE_DOUBLE;
			m_fval=(float)m_val;
			m_fval-=val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val-=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val-=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			type=VAR_TYPE_DOUBLE;
			m_fval=(float)m_val;
			m_fval-=val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			type=VAR_TYPE_DOUBLE;
			m_fval-=(float)val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_DOUBLE;
			m_fval-=(float)val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			m_fval-=val.m_fval;
			break;
		}
		break;
	}
	return *this;
}


var& var::operator -=(int v)
{
	var a;
	a=v;

	return operator -=(a);
}

var& var::operator -=(double v)
{
	var a;
	a=v;

	return operator -=(a);
}

//
// operator *=
//


var& var::operator *=(const var& val)
{
	switch(type){
	case 0:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val*=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val*=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			type=VAR_TYPE_DOUBLE;
			m_fval=(float)m_val;
			m_fval*=val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val*=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val*=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			type=VAR_TYPE_DOUBLE;
			m_fval=(float)m_val;
			m_fval*=val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			type=VAR_TYPE_DOUBLE;
			m_fval*=(float)val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_DOUBLE;
			m_fval*=(float)val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			m_fval*=val.m_fval;
			break;
		}
		break;
	}
	return *this;
}


var& var::operator *=(int v)
{
	var a;
	a=v;

	return operator *=(a);
}

var& var::operator *=(double v)
{
	var a;
	a=v;

	return operator *=(a);
}

//
// operator /=
//


var& var::operator /=(const var& val)
{
	switch(type){
	case 0:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val/=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val/=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			type=VAR_TYPE_DOUBLE;
			m_fval=(float)m_val;
			m_fval/=val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val/=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val/=val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			type=VAR_TYPE_DOUBLE;
			m_fval=(float)m_val;
			m_fval/=val.m_fval;
			break;
		}
		break;
	case VAR_TYPE_DOUBLE:
		switch(val.type){
		case 0:
			type=VAR_TYPE_DOUBLE;
			m_fval/=(float)val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_DOUBLE;
			m_fval/=(float)val.m_val;
			break;
		case VAR_TYPE_DOUBLE:
			m_fval/=val.m_fval;
			break;
		}
		break;
	}
	return *this;
}


var& var::operator /=(int v)
{
	var a;
	a=v;

	return operator /=(a);
}

var& var::operator /=(double v)
{
	var a;
	a=v;

	return operator /=(a);
}


//
// operator %=
//


var& var::operator %=(const var& val)
{
	switch(type){
	case 0:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val%=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val%=val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val%=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val%=val.m_val;
			break;
		}
		break;
	}
	return *this;
}


var& var::operator %=(int v)
{
	var a;
	a=v;

	return operator %=(a);
}


//
// operator &=
//


var& var::operator &=(const var& val)
{
	switch(type){
	case 0:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val&=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val&=val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val&=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val&=val.m_val;
			break;
		}
		break;
	}
	return *this;
}


var& var::operator &=(int v)
{
	var a;
	a=v;

	return operator &=(a);
}


//
// operator |=
//


var& var::operator |=(const var& val)
{
	switch(type){
	case 0:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val|=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val|=val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val|=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val|=val.m_val;
			break;
		}
		break;
	}
	return *this;
}


var& var::operator |=(int v)
{
	var a;
	a=v;

	return operator |=(a);
}

//
// operator >>=
//


var& var::operator >>=(const var& val)
{
	switch(type){
	case 0:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val>>=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val>>=val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val>>=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val>>=val.m_val;
			break;
		}
		break;
	}
	return *this;
}


var& var::operator >>=(int v)
{
	var a;
	a=v;

	return operator >>=(a);
}


//
// operator <<=
//


var& var::operator <<=(const var& val)
{
	switch(type){
	case 0:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val<<=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val<<=val.m_val;
			break;
		}
		break;
	case VAR_TYPE_INT:
		switch(val.type){
		case 0:
			type=VAR_TYPE_INT;
			m_val<<=val.m_val;
			break;
		case VAR_TYPE_INT:
			type=VAR_TYPE_INT;
			m_val<<=val.m_val;
			break;
		}
		break;
	}
	return *this;
}


var& var::operator <<=(int v)
{
	var a;
	a=v;

	return operator <<=(a);
}


//
// typeof
//



var typeof(int a)
{
	var ret;
	ret="number";
	return ret;
}
var typeof(double a)
{
	var ret;
	ret="number";
	return ret;
}

var typeof(var& a)
{
	var ret;
	switch(a.type){
	case VAR_TYPE_NULL:
		ret="undefined";
		break;
	case VAR_TYPE_INT:
	case VAR_TYPE_DOUBLE:
		ret="number";
		break;
	case VAR_TYPE_STRING:
		ret="string";
		break;
	case VAR_TYPE_LIST:
	case VAR_TYPE_MAP:
		ret="object";
		break;
	case VAR_TYPE_FUNCTION:
		ret = "function";
		break;
	}
	return ret;
}
var typeof(char* a)
{
	var ret;
	ret="string";
	return ret;
}


std::string var::charAt(int a)
{
	std::string ret;
	std::string tmp;
	ret="";
	tmp=toString();
	if(a<0)return ret;
	if(a>=(int)tmp.length())return ret;
	ret=tmp.substr(a,1);
	
	return ret;
}

int var::charCodeAt(int a)
{
	int ret=-1;
	std::string tmp;
	tmp=toString();
	if(a<0)return ret;
	if(a>=(int)tmp.length())return ret;
	ret=tmp.at(a);
	
	return ret;
}


std::string String::fromCharCode(int a)
{
	std::string ret;
	char buf[16];
	ret="";
	if(a<0 || a>255)return ret;
	buf[0]=a;
	buf[1]=0;
	ret=buf;
	return ret;
}

std::string String::fromCharCode(int a,int b)
{
	std::string ret;
	char buf[16];
	ret="";
	if(a<0 || a>255)return ret;
	buf[0]=a;
	buf[1]=b;
	buf[2]=0;
	ret=buf;
	return ret;
}
std::string String::fromCharCode(int a,int b,int c)
{
	std::string ret;
	char buf[16];
	ret="";
	if(a<0 || a>255)return ret;
	buf[0]=a;
	buf[1]=b;
	buf[2]=c;
	buf[3]=0;
	ret=buf;
	return ret;
}
std::string String::fromCharCode(int a,int b,int c,int d)
{
	std::string ret;
	char buf[16];
	ret="";
	if(a<0 || a>255)return ret;
	buf[0]=a;
	buf[1]=b;
	buf[2]=c;
	buf[3]=d;
	buf[4]=0;
	ret=buf;
	return ret;
}


int var::indexOf(std::string a)
{
	int ret=-1;
	std::string tmp;
	tmp=toString();
	ret=tmp.find(a);
	return ret;
}

int var::lastIndexOf(std::string a)
{
	int ret=-1;
	std::string tmp;
	tmp=toString();
	ret=tmp.rfind(a);
	return ret;
}

var parseInt(var a)
{
	var ret=NaN;
	int t,v=0;
	std::string tmp;
	if (a == null)return ret;

	tmp=a.toString();
	t=sscanf(tmp.c_str(),"%d",&v);
	if (t == 1)ret = v;
	return ret;
}
var parseFloat(var a)
{
	var ret = NaN;
	int t;
	double v = 0.0;
	std::string tmp;
	if (a == null)return ret;

	tmp = a.toString();
	t=sscanf(tmp.c_str(),"%lf",&v);
	if (t == 1)ret = v;
	return ret;
}

std::string  var::toLowerCase()
{
	std::string tmp;
	tmp=toString();
	transform(tmp.begin(),tmp.end(),tmp.begin(),tolower);
	return tmp;
}

std::string  var::toUpperCase()
{
	std::string tmp;
	tmp=toString();
	transform(tmp.begin(),tmp.end(),tmp.begin(),toupper);
	return tmp;
}

std::string  var::trim()
{
	std::string string;
	std::string result;
	const char* trimCharacterList = " \t\v\r\n";
	string=toString();

	std::string::size_type left = string.find_first_not_of(trimCharacterList);
	if (left != std::string::npos){
		std::string::size_type right = string.find_last_not_of(trimCharacterList);
		result = string.substr(left, right - left + 1);
	}
	return result;
}

std::string  var::replace(std::string from,std::string to)
{
	std::string str;
	str=toString();

	std::string::size_type pos = str.find(from);
	//while(pos != std::string::npos){
	if(pos != std::string::npos){
		str.replace(pos, from.size(), to);
		pos = str.find(from, pos + to.size());
	}
	return str;
}

std::string  var::substr(int a)
{
	std::string str;
	std::string ret;

	ret="";
	str=toString();
	if(a<0)a=str.length()+a;
	if(a<0)a=0;

	if((int)str.length()<=a)return ret;
	ret=str.substr(a);
	return ret;
}

std::string  var::substr(int a,int b)
{
	std::string str;
	std::string ret;
	int l;

	ret="";
	str=toString();
	if(a<0)a=str.length()+a;
	if(a<0)a=0;

	l=str.length();

	if(l<=a)return ret;
	if(l<=a+b)b=l-a;
	if(b<1)return ret;
	ret=str.substr(a,b);
	return ret;
}

std::string  var::substring(int a)
{
	std::string str;
	std::string ret;

	if(a<0)a=0;
	ret="";
	str=toString();
	if((int)str.length()<=a)return ret;
	ret=str.substr(a);
	return ret;
}

std::string  var::substring(int a,int c)
{
	std::string str;
	std::string ret;
	int b=0,l=0;
	ret="";
	if(a<0)a=0;
	if(b<0)b=0;

	if(c<a){
		b=c;
		c=a;
		a=b;
	}
	b=c-a;

	str=toString();
	l=str.length();

	if(l<=a)return ret;
	if(l<=a+b)b=l-a;
	if(b<1)return ret;
	ret=str.substr(a,b);
	return ret;
}



void var::delete_(std::string str)
{
	var* a;
	std::map<std::string,var*>::iterator it;

	if(type!=VAR_TYPE_MAP)return;

	it=m_map.find(str);
	if(it==m_map.end())return;
	a=it->second;
	m_map.erase(str);
	delete a;
}


void sessionStorage::setItem(std::string key, std::string val)
{
	FILE* fp;
	std::string fname;
	fname = "sessionstorage_"+key+".txt";
	fp = fopen(fname.c_str(), "wt");
	if (fp == NULL)return;
	fputs(val.c_str(),fp);
	fclose(fp);
}

std::string  sessionStorage::getItem(std::string key)
{
	std::string ret;
	char buf[4096];

	FILE* fp;
	std::string fname;
	fname = "sessionstorage_" + key + ".txt";
	fp = fopen(fname.c_str(), "rt");
	if (fp == NULL)return ret;
	buf[0] = 0;
	fgets(buf,sizeof(buf), fp);
	ret = buf;
	fclose(fp);
	return ret;
}

void sessionStorage::removeItem(std::string key)
{
	std::string fname;
	fname = "sessionstorage_" + key + ".txt";
	remove(fname.c_str());
}

void localStorage::setItem(std::string key, std::string val)
{
	FILE* fp;
	std::string fname;
	fname = "localstorage_" + key + ".txt";
	fp = fopen(fname.c_str(), "wt");
	if (fp == NULL)return;
	fputs(val.c_str(), fp);
	fclose(fp);
}

void localStorage::removeItem(std::string key)
{
	std::string fname;
	fname = "localstorage_" + key + ".txt";
	remove(fname.c_str());
}

std::string  localStorage::getItem(std::string key)
{
	std::string ret;
	char buf[4096];

	FILE* fp;
	std::string fname;
	fname = "localstorage_" + key + ".txt";
	fp = fopen(fname.c_str(), "rt");
	if (fp == NULL)return ret;
	buf[0] = 0;
	fgets(buf, sizeof(buf), fp);
	ret = buf;
	fclose(fp);
	return ret;
}

std::string encodeURIComponent(std::string str)
{
	std::string ret;
	int i,l;
	unsigned char* p;
	int c;
	char tmp[8];
	char *map = "0123456789ABCDEFG";
	int f;

	l = str.length();
	p = (unsigned char*)(str.c_str());
	for (i = 0; i < l; i++){
		c = p[i];
		f = 0;
		if (c >= '0' && c <= '9')f = 1;
		if (c >= 'A' && c <= 'Z')f = 1;
		if (c >= 'a' && c <= 'z')f = 1;
		if (c == '-' || c == '_' || c == '.' || c == '!' || c == '~' || 
			c == '*' || c == '\'' || c == '(' || c == ')' )f = 1;
		if (f){
			tmp[0] = c;
			tmp[1] = 0;
		}else{
			tmp[0] = '%';
			tmp[1] = map[(c>>4)];
			tmp[2] = map[(c&15)];
			tmp[3] = 0;
		}
		ret += tmp;
	}
	return ret;
}

std::string decodeURIComponent(std::string str)
{
	std::string ret;
	int i, l;
	unsigned char* p;
	char tmp[8];
	int c;
	int k;

	l = str.length();
	p = (unsigned char*)(str.c_str());
	for (i = 0; i < l; i++){
		c = p[i];
		if (c == '%'){
			if (i + 2 < l){
				tmp[0] = p[i + 1];
				tmp[1] = p[i + 2];
				tmp[2] = 0;
				k = 0;
				sscanf(tmp,"%x", &k);
				tmp[0] = k;
				tmp[1] = 0;
				i += 2;
			}
			else{
				//error
				ret = "";
				return ret;
			}
		}
		else{
			tmp[0] = c;
			tmp[1] = 0;
		}
		ret += tmp;
	}
	return ret;
}

var typeof(class sessionStorage& a)
{
	var ret;
	ret = "object";
	return ret;
}
var typeof(class localStorage& a)
{
	var ret;
	ret = "object";
	return ret;
}

bool isNaN(var a)
{
	if (a.type == VAR_TYPE_NAN)return true;
	return false;
}

#endif /*  JAVASCRIPT */


var& var::operator =(void (*f)(void))
{
	clear();
	this->type = VAR_TYPE_FUNCTION;
	this->m_func2 = f;
	this->m_func1 = NULL;
	this->m_func3 = NULL;
	this->m_func4 = NULL;
	return *this;
}

var& var::operator =(var(*f)(void))
{
	clear();
	this->type = VAR_TYPE_FUNCTION;
	this->m_func1 = f;
	this->m_func2 = NULL;
	this->m_func3 = NULL;
	this->m_func4 = NULL;
	return *this;
}

var& var::operator =(void(*f)(var a))
{
	clear();
	this->type = VAR_TYPE_FUNCTION;
	this->m_func4 = f;
	this->m_func1 = NULL;
	this->m_func2 = NULL;
	this->m_func3 = NULL;
	return *this;
}

var& var::operator =(var(*f)(var a))
{
	clear();
	this->type = VAR_TYPE_FUNCTION;
	this->m_func3 = f;
	this->m_func1 = NULL;
	this->m_func2 = NULL;
	this->m_func4 = NULL;
	return *this;
}

var var::operator()()
{
	var ret,a;
	if (this->m_func2){
		(this->m_func2)();
	}else if (this->m_func1){
		ret=(this->m_func1)();
	}else if (this->m_func4){
		(this->m_func4)(a);
	}else if (this->m_func3){
		ret = (this->m_func3)(a);
	}
	return ret;
}

var var::operator()(var a)
{
	var ret;
	if (this->m_func2){
		(this->m_func2)();
	}else if (this->m_func1){
		ret=(this->m_func1)();
	}else if (this->m_func4){
		(this->m_func4)(a);
	}else if (this->m_func3){
		ret = (this->m_func3)(a);
	}
	return ret;
}


var::var(void (*f)())
{
	init();
	this->type = VAR_TYPE_FUNCTION;
	this->m_func2 = f;
	this->m_func1 = NULL;
	this->m_func3 = NULL;
	this->m_func4 = NULL;
}

var::var(var(*f)())
{
	init();
	this->type = VAR_TYPE_FUNCTION;
	this->m_func1 = f;
	this->m_func2 = NULL;
	this->m_func3 = NULL;
	this->m_func4 = NULL;
}

var::var(void(*f)(var a))
{
	init();
	this->type = VAR_TYPE_FUNCTION;
	this->m_func4 = f;
	this->m_func1 = NULL;
	this->m_func2 = NULL;
	this->m_func3 = NULL;
}

var::var(var(*f)(var a))
{
	init();
	this->type = VAR_TYPE_FUNCTION;
	this->m_func3 = f;
	this->m_func1 = NULL;
	this->m_func2 = NULL;
	this->m_func4 = NULL;
}

var::var(class NaN n)
{
	init();
	this->type = VAR_TYPE_NAN;
}

var& var::operator =(class NaN n)
{
	clear();
	this->type = VAR_TYPE_NAN;
	return *this;
}


int Math::floor(var a)
{
	int ret = 0;
	ret = (int)::floor((double)a);
	return ret;
}

int Math::round(var a)
{
	int ret = 0;
	ret = (int)::round((double)a);
	return ret;
}

int Math::trunc(var a)
{
	int ret = 0;
	ret = (int)::trunc((double)a);
	return ret;
}


double Math::abs(var a)
{
	double ret = 0;
	ret = ::fabs((double)a);
	return ret;
}

double Math::sin(var a)
{
	double ret = 0;
	ret = ::sin((double)a);
	return ret;
}

double Math::cos(var a)
{
	double ret = 0;
	ret = ::cos((double)a);
	return ret;
}

double Math::tan(var a)
{
	double ret = 0;
	ret = ::tan((double)a);
	return ret;
}

double Math::random()
{
	double ret = 0;

	ret = ::rand();
	ret /= RAND_MAX;
	return ret;
}


double Math::exp(var a)
{
	double ret = 0;
	ret = ::exp((double)a);
	return ret;
}

double Math::pow(var a,var b)
{
	double ret = 0;
	ret = ::pow((double)a, (double)b);
	return ret;
}


double Math::sqrt(var a)
{
	double ret = 0;
	ret = ::sqrt((double)a);
	return ret;
}

double Math::log(var a)
{
	double ret = 0;
	ret = ::log((double)a);
	return ret;
}

double Math::log10(var a)
{
	double ret = 0;
	ret = ::log10((double)a);
	return ret;
}

//
// iterator
//


#ifdef  I_USE_ITERATOR

varIterator::varIterator()
{
	m_var = NULL;
	m_index = 0;
}

varIterator::varIterator(var* myClass, int index)
{
	m_var = myClass;
	m_index = index;
}

varIterator::varIterator(const varIterator& iterator)
{
	m_var = iterator.m_var;
	m_index = iterator.m_index;
}

var varIterator::operator*()
{
	var dummy;
	std::map<std::string, var*>::iterator ite;
	std::string str;
	int ct = 0;
	for (ite = m_var->m_map.begin(); ite != m_var->m_map.end(); ite++)
	{
		if (ct == m_index){
			str = ite->first;
			dummy = str;
			break;
		}
		ct++;
	}
	return dummy;
}

varIterator& varIterator::operator++()
{
	m_index++;
	return *this;
}

varIterator varIterator::operator++(int)
{
	varIterator result = *this;
	m_index++;
	return result;
}

bool varIterator::operator!=(const varIterator& iterator)
{
	return this->m_var != iterator.m_var || this->m_index != iterator.m_index;
}

bool varIterator::operator==(const varIterator& iterator)
{
	return !(*this != iterator);
}

var::iterator var::begin()
{
	return varIterator(this, 0);
}

var::iterator var::end()
{
	int len=0;

	//len = strlen(m_str);
	if (this->type == VAR_TYPE_MAP){
		len = m_map.size();
	}
	return varIterator(this, len);
}


#endif /* I_USE_ITERATOR */



//
//
//
#if 0
#ifndef JAVASCRIPT
void variant_main(){
#endif


	var a,b,c,d,e,f;

	var g=5;
	var h="hoge";
	var j="hoge";
	var k=j;
	var l=4.0;

	var str;
	var i=-1;

	//null testing 
	if(a==null)println("a is null  #### OK ####");
	else println("a is NOT null#### NG ####");

	if(a!=null)println("a is NOT null #### NG ####\n");
	else println("a is null#### OK ####\n");

	a=new Array();

	a["key"]="value";
	a[5]=10;


	if(a==null)println("a is null #### NG ####\n");
	else println("a is NOT null#### OK ####\n");

	if(a!=null)println("a is NOT null#### OK ####\n");
	else println("a is null#### NG ####\n");

	//int testing
	i=a[5];
	//println("i=%d\n",i);
	if(i==10)println("#### OK ####\n");
	else println("#### NG ####\n");


	// redbrack testing
	b=new Array();
	b["hoge"]=clone(a);
	c=clone(b);
	//c=b;

	d=c["hoge"];

	if(a["key"]==e)println("a[key]is e #### NG ####\n");
	else println("a[key]is NOT e #### OK ####\n");

	if(a["key"]!=e)println("a[key]is NOT e #### OK ####\n");
	else println("a[key]is e #### NG ####\n");

	if(a[5]==f)println("a[key]is f #### NG ####\n");
	else println("a[key]is NOT f #### OK ####\n");


	if(a[5]!=f)println("a[key]is NOT f #### OK ####\n");
	else println("a[key]is f #### NG ####\n");


	e="value";
	f=10;
	//println("str=>>%s<<\n",((std::string)e).c_str());
	if(e=="value")println("####  OK ####\n");
	else println("####  NG ####\n");

	if(e!="value")println("####  NG ####\n");
	else println("####  OK ####\n");

	//println("str=>>%s<<\n",e.pchar());
	//if(e.pchar()=="value")println("#### OK ####\n");
	//else println("####  NG ####\n");

	if(a["key"]==e)println("a[key]is e #### OK ####\n");
	else println("a[key]is NOT e #### NG ####\n");

	if(a[5]==f)println("a[key]is f #### OK ####\n");
	else println("a[key]is NOT f #### NG ####\n");


	str=a["key"];
	a["key"]="hoge!!!!";
	//println("str=>>%s<<\n",str.c_str());
	if(str=="value")println("#### OK ####\n");
	else println("#### NG ####\n");

	str=d["key"];
	println(str);
	if(str=="value")println("001 #### OK ####\n");
	else println("001 #### NG ####\n");


	str=a["key"];
	//println("str=>>%s<<\n",str.c_str());
	if(str=="hoge!!!!")println("#### OK ####\n");
	else println("#### NG ####\n");

	str=a[5];
	//println("str=>>%s<<\n",str.c_str());
	if(str=="10")println("#### OK ####\n");
	else println("#### NG ####\n");

	a["key"]="123";
	i=a["key"];
	//println("i=%d\n",i);
	if(i==123)println("123 #### OK ####\n");
	else println("123 #### NG ####\n");

	a["key"]=0;
	i=a["key"];
	//println("i=%d\n",i);
	if(i==0)println("#### OK ####\n");
	else println("#### NG ####\n");


	// clear testing
	a["key"]=100;
	//a["key"].clear();
	a["key"]=0;

	i=a["key"];
	//println("i=%d\n",i);
	if(i==0)println("100 #### OK ####\n");
	else println("100 #### NG ####\n");


	// length tesintg 
	// a is object javascript is NG
	a=new Array();
	if(a!=null)println("101 #### OK ####\n");
	else println("101 #### NG ####\n");

	//i=undefined
	i=a.length();
	//println("102 a.length()>>"+i+"<<");
	if(i==0)println("102 #### OK ####\n");
	else println("102 #### NG ####\n");

	a=-5;
	i=a.length();
	//println("a.length()=%d\n",i);
	if(i==5)println("103 #### OK ####\n");
	else println("103 #### NG ####\n");

	a="";
	i=a.length();
	//println("a.length()=%d\n",i);
	if(i==0)println("104 #### OK ####\n");
	else println("104 #### NG ####\n");

	a="abcdefg";
	i=a.length();
	//println("a.length()=%d\n",i);
	if(i==7)println("105 #### OK ####\n");
	else println("105 #### NG ####\n");

	a=new Array();
	a[7]=70;
	a[100]=123;
	i=a.length();
	//println("a.length()=%d\n",i);
	if(i==101)println("106 #### OK ####\n");
	else println("106 #### NG ####\n");

	// c++ 1 js 101  NG
	a[-1]=123;
	i=a.length();
	if(i==3)println("107 #### OK ####\n");
	else println("107 #### NG ####\n");

#ifndef JAVASCRIPT
	// multidimensional array testing
	a.clear();
	a[15][5]=55;
	i=a[15].length();
	//println("a[15].length()=%d\n",i);
	if(i==6)println("#### OK ####\n");
	else println("#### NG ####\n");

	i=a[15][5];
	//println("i=%d\n",i);
	if(i==55)println("#### OK ####\n");
	else println("#### NG ####\n");
#endif

	//erase test
	a=new Array();
	a["sss"]=5;
	a["ttt"]=10;
	a["uuu"]=20;
	DELETE(a,"tppp");
	DELETE(a,"ttt");
	DELETE(a,"ttt");
	i=a.length();
	println("i="+i);
	if(i==2)println("#### OK ####\n");
	else println("#### NG ####\n");
	b=a["ttt"];
	println(b);
	if(b==null)println("111 #### OK ####\n");
	else println("111 #### NG ####\n");

	a=new Array();
	a[12]=5;
	b=clone(a);
	c=b[12];
	println(c);

	b=a[3];
	println(b);
	if(b==null)println("112 #### OK ####\n");
	else println("112 #### NG ####\n");


#ifndef JAVASCRIPT
}
#endif
#endif



#if 0
#ifndef JAVASCRIPT
void variant_main(){
#endif

	var a=5.9;
	var b;
	b=a;

#ifndef JAVASCRIPT
	std::string h,i;
	var c,d,e,f,g;
#else
	var c={},d,e,f,g;
	var h,i;
#endif

	d=0.3;
	c[0]=5;
	c[d]=4.8;
	e=c[d];
	f=4.8;
	g=5.9;
	h="4.8";
	i="5.8";

	if(e==f)println("OK1");
	if(e!=g)println("OK2");
	if(e==4.8)println("OK3");
	if(e=="4.8")println("OK4");

	if(e==h)println("OK5");
	if(e!=i)println("OK6");

	var aa=true;
	aa=false;
	if(aa==false)println("OK7");
	if(aa!=true)println("OK8");

	var j,k,l;
	j+=k;
//	k=2;
//	j+=k;
//	k=4.7;
//	j+=k;
	k="abcdef";
	j="01234";
	l=k+j;
	println(l);
	if(l=="abcdef01234")println("OK9");
	var ba,bb,bc;
	ba="2.2";
	bb=1.4;
	bc=ba+bb;
	println(bc);
	if(bc=="2.21.4")println("OK10");

#ifndef JAVASCRIPT
	double da=1.0,db=1.0;
#else
	var da=1.0,db=1.0;
#endif

	da++;
	if(da==2)println("OK11");
	var ca,cb;

	cb=5.0;
	cb++;
	cb--;
	++cb;
	--cb;
	cb++;
	++cb;

	ca=-cb;
	ca=+cb;
	ca=!cb;
	ca=~cb;


#ifndef JAVASCRIPT
}
#endif
#endif


#if 0
#ifndef JAVASCRIPT
void variant_main(){
#endif

	var a;
	if(a==null)println("1. OK");
	else println("1. NG");
	if(a!=null)println("2. NG");
	else println("2. OK");
	a=null;
	if(a==null)println("3. OK");
	else println("3. NG");
	if(a!=null)println("4. NG");
	else println("4. OK");
	a=0;
	if(a==null)println("5. NG");
	else println("5. OK");
	if(a!=null)println("6. OK");
	else println("6. NG");

	a="";
	if(a==null)println("7. NG");
	else println("7. OK");
	if(a!=null)println("8. OK");
	else println("8. NG");

	a="";
	if(a==0)println("9. OK");
	else println("9. NG");
	if(a!=0)println("10. NG");
	else println("10. OK");

	a="";
	if(a=="")println("11. OK");
	else println("11. NG");
	if(a!="")println("12. NG");
	else println("12. OK");

	a=0;
	if(a=="")println("13. OK");
	else println("13. NG");
	if(a!="")println("14. NG");
	else println("14. OK");

	a=new Array();
	if(a==null)println("15. NG");
	else println("15. OK");
	if(a!=null)println("16. OK");
	else println("16. NG");
	if(a[1]==null)println("17. OK");
	else println("17. NG");
	if(a[1]!=null)println("18. NG");
	else println("18. OK");
	if(a==null)println("19. NG");
	else println("19. OK");
	if(a!=null)println("20. OK");
	else println("20. NG");

	a[1]=0;
	if(a[1]==null)println("21. NG");
	else println("21. OK");
	if(a[1]!=null)println("22. OK");
	else println("22. NG");


#ifndef JAVASCRIPT
}
#endif
#endif


#if 0
#ifndef JAVASCRIPT
void variant_main(){
#endif

	var a;
#ifdef JAVASCRIPT
	var b=[[1,2,3,4,5]];
#else
	var b[][6]={{1,2,3,4,5}};
#endif
	var i,j,k,l;
	a=new Array();
	for(i=0;i<2;i++){
		a[i]=new Array();
		for(j=0;j<4;j++){
			a[i][j]=i*100+j;
			// kore ha dame
			//a[i][j]=100*i+j;

			//OK
			// var*int var+int
			//NG
			// int*var int+var
		}
	}

	l=123;
	k="sdsds"+l.toString();
	println(k);

	for(i=0;i<2;i++){
		for(j=0;j<4;j++){
			println(a[i][j]);
		}
	}
	for(i=0;i<6;i++){
		println(b[0][i]);
	}



#ifndef JAVASCRIPT
}
#endif
#endif



#if 0
#ifndef JAVASCRIPT
void variant_main(){
#endif

var str;
var a=new Array();
var b="hoge";
var c=new Array();

a["key"]=b;
b="aaaa";

c=clone(a);

str="";
str=str+"c[key]="+c["key"];
println(str);

if(c["key"]=="hoge")println("1. OK");
else println("1. NG");

c["key"]=b;

str="";
str=str+"a[key]="+a["key"];
println(str);
str="";
str=str+"c[key]="+c["key"];
println(str);

if(c["key"]=="aaaa" && a["key"]=="hoge")println("2. OK");
else println("2. NG");


#ifndef JAVASCRIPT
}
#endif
#endif

#if 0
#if !defined(JAVASCRIPT) 

int main(int argc,char* argv[])
{
#if defined(_WIN32) && !defined(__GNUC__)
//	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_WNDW);
//	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
//	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#endif
	variant_main();
}
#endif
#endif

