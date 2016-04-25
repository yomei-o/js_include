#ifndef JAVASCRIPT
#include <stdio.h>
#include "variant.h"
#include "js_base64.h"
#endif

#ifndef JAVASCRIPT
#if defined(_WIN32) && !defined(__GNUC__)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif /* _WIN32 */
#endif


#ifndef JAVASCRIPT
#define null 0
#define true 1
#define false 0
#endif

#ifdef JAVASCRIPT 
#define BEGIN_TRY() try{
#define END_TRY() }catch(e){}
#else
#define BEGIN_TRY()
#define END_TRY()
#endif



#ifdef JAVASCRIPT
strarray_println = function(src)
#else
void strarray_println(var src)
#endif
{
#ifdef JAVASCRIPT
	alert(src);
#else
	printf("%s\n", ((std::string)src).c_str());
#endif
}


var strarray_str = "0123456789abcdef";

#ifdef JAVASCRIPT
strarray_c2hexstr = function(v)
#else
var strarray_c2hexstr(var v)
#endif
{
	var ret = "";
	var a;
	v = v & 255;
	a = (v >> 4) & 15;
	ret += strarray_str.substring(a, a + 1);
	a = v & 15;
	ret += strarray_str.substring(a, a + 1);
	return ret;
}

#ifdef JAVASCRIPT
strarray_i2hexstr = function(v)
#else
var strarray_i2hexstr(var v)
#endif
{
	var ret = "";
	var a;
	a = (v >> 24) & 255;
	ret += strarray_c2hexstr(a);
	a = (v >> 16) & 255;
	ret += strarray_c2hexstr(a);
	a = (v >> 8) & 255;
	ret += strarray_c2hexstr(a);
	a = v& 255;
	ret += strarray_c2hexstr(a);
	return ret;
}

#ifdef JAVASCRIPT
strarray_array2hexstr = function(v)
#else
var strarray_array2hexstr(var v)
#endif
{
	var ret = "";
	var a ,i, l;

#ifdef JAVASCRIPT
	l = v.length;
#else
	l = v.length();
#endif
	for (i = 0; i < l; i++){
		a = v[i];
		ret += strarray_c2hexstr(a);
	}
	return ret;
}

#ifdef JAVASCRIPT
strarray_arraysize = function(v)
#else
var strarray_arraysize(var v)
#endif
{
	var l;
#ifdef JAVASCRIPT
	l = v.length;
#else
	l = v.length();
#endif
	return l;
}

#ifdef JAVASCRIPT
strarray_ccode2i = function(v)
#else
var strarray_ccode2i(var v)
#endif
{
	if (v >= 0x30 && v <= 0x39)return v - 0x30;
	if (v >= 0x41 && v <= 0x46)return v - 0x41+10;
	if (v >= 0x61 && v <= 0x66)return v - 0x61+10;

	return 0;	
}

#ifdef JAVASCRIPT
strarray_hexstr2c = function(v)
#else
var strarray_hexstr2c(var v)
#endif
{
	var ret = 0;
	var a,b,l;
	v=v.toLowerCase();
#ifdef JAVASCRIPT
	l = v.length;
#else
	l = v.length();
#endif
	if (l < 2){
		a = 0x30;
		b = v.charCodeAt(0);
	}
	else{
		a = v.charCodeAt(0);
		b = v.charCodeAt(1);
	}
	a = strarray_ccode2i(a);
	b = strarray_ccode2i(b);
	return a*16+b;
}


#ifdef JAVASCRIPT
strarray_hexstr2array = function(v)
#else
var strarray_hexstr2array(var v)
#endif
{
	var l,i,ret;
	ret=new Array();

#ifdef JAVASCRIPT
	l = v.length;
#else
	l = v.length();
#endif
	i = "";
	if(l%2==1)i="0";
	v = i + v;
	v = v.toLowerCase();
	l++;
	l /= 2;

	for (i = 0; i < l; i ++ ){
		ret[i] = strarray_hexstr2c(v.substring(i*2,i*2+2));
	}
	return ret;
}


#ifdef JAVASCRIPT
strarray_test = function()
#else
void strarray_test()
#endif
{
	var a;
	var v = 0x12345678;
	var str;
	str = strarray_i2hexstr(v);

	v = new Array();
	v[0] = 0x12;
	v[1] = 0x34;
	str = strarray_array2hexstr(v);

	v=strarray_arraysize(v);
	strarray_println(v);

	v=strarray_ccode2i(0x46);
	v = strarray_hexstr2c("f");
	v = strarray_hexstr2c("1f");


	v = strarray_hexstr2array("12345");
	a = strarray_arraysize(v);
	strarray_println(a);

	a = v[0];
	a = v[1];
	a = v[2];

	v = strarray_hexstr2array("abcdef");
	a = strarray_arraysize(v);
	strarray_println(a);

	a = v[0];
	a = v[1];
	a = v[2];
}

