#ifdef JAVASCRIPT
//// JavaScript include library  Rev 0.04 ----BEGIN----
if(typeof include=="undefined"){var t,q="__QUERY__";include_list={};include=function(s){if(s.indexOf(q)!=-1){
if(typeof sessionStorage!="undefined" && (t=sessionStorage.getItem(q)))s=s.replace(q,t);
else s=s.replace(q,location.search);}
var xhr=null;if(include_list[s])return;include_list[s]=1;if (window.XMLHttpRequest)xhr=new XMLHttpRequest();
else if(window.ActiveXObject)try{xhr=new ActiveXObject("Msxml2.XMLHTTP");}catch(e){xhr=new ActiveXObject("Microsoft.XMLHTTP");}
try{xhr.open("GET",s,false);xhr.send("");(0,eval)(xhr.responseText);}catch(e){}}}declare_var_begin=declare_var_end=function(){};
//// JavaScript include library ----END----
#endif

#ifdef JAVASCRIPT
include("json_sample.exe__QUERY__");
include("jquery-1.11.3.js");
include("json_sample.js");
//include("json_sample.exe__QUERY__");
include("js_base64.js");
#else
#include "variant.h"
#include "js_include.h"
#include "json_sample.h"
#include "js_base64.h"
#include "js_md5.h"
#include "js_sha1.h"
#include "js_strarray.h"
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


#ifndef JAVASCRIPT 
#define __QUERY__ "?aaa=000"
#endif
//
// Debufg print
//

#ifdef JAVASCRIPT
println = function(src)
#else
void println(var src)
#endif
{
#ifdef JAVASCRIPT
	alert(src);
#else
	printf("%s\n", ((std::string)src).c_str());
#endif
}

#ifndef JAVASCRIPT
void println(const char* a)
{
	var aa;
	aa = a;
	println(aa);
}
#endif

//
// length
//


#ifdef JAVASCRIPT
strlength = function(src)
#else
var strlength(var src)
#endif
{
	var ret=0;
	if (src == null || src == "")return ret;
#ifdef JAVASCRIPT
	ret=src.length;
#else
	ret = src.length();
#endif
	return ret;
}

#ifndef JAVASCRIPT
void strlength(const char* a)
{
	var aa;
	aa = a;
	strlength(aa);
}
#endif


//
// QUERY
//

#ifdef JAVASCRIPT
getQuery = function()
#else
var getQuery()
#endif
{
	var ret;
	ret= "";
	if (typeof(sessionStorage) != "undefined"){
		ret = sessionStorage.getItem("__QUERY__");
		if (ret!=null)return ret;
	}
#ifdef JAVASCRIPT
	if(location.search)ret=location.search;
#else
#ifdef __QUERY__
	ret = __QUERY__;
#endif
#endif
	return ret;
}

#ifdef JAVASCRIPT
setQuery = function(src)
#else
void setQuery(var src)
#endif
{
	if (src == null || src.indexOf("?") != 0)return;
	if (typeof(sessionStorage) != "undefined"){
		sessionStorage.setItem("__QUERY__",src);
	}
#ifdef JAVASCRIPT
	__QUERY__ = src;
#endif
}

#ifdef JAVASCRIPT
jumpLocation = function(src)
#else
void jumpLocation(var src)
#endif
{
	if(src==null)return;
#ifdef JAVASCRIPT
	if(typeof(__QUERY__)!="undefined")src+=__QUERY__;
	location.href=src;
#else
	println("jumpLocation()");
	println(src);
#endif
}


#ifdef JAVASCRIPT
myName = function()
#else
var myName()
#endif
{
	var ret="";
	var idx = -1;

#ifdef JAVASCRIPT
	ret = location.href;
#else
	ret = __FILE__;
#endif
	idx = ret.lastIndexOf("/");
	if (idx != -1)ret = ret.substring(idx + 1);
	idx = ret.lastIndexOf("\\");
	if (idx != -1)ret = ret.substring(idx + 1);
	idx = ret.lastIndexOf("?");
	if (idx != -1)ret = ret.substring(0,idx);
	return ret;
}

#ifdef JAVASCRIPT
getQueryValue = function(src,key)
#else
var getQueryValue(var src,var key)
#endif
{
	var ret="";
	var idx;
	if (src == null || src == "")return ret;
	if (key == null || key == "")return ret;
	do{
		idx = src.indexOf("+");
		if (idx == -1)break;
		src = src.replace("+", "%20");
	} while (true);
	key = key + "=";
	idx = src.indexOf(key);
	if (idx == -1)return ret;
	ret = src.substring(idx + strlength(key));
	idx = ret.indexOf("&");
	if (idx != -1)ret = ret.substring(0, idx);
	ret = decodeURIComponent(ret);
	return ret;
}

#ifdef JAVASCRIPT
createQueryValue = function(key,val)
#else
var createQueryValue(var key,var val)
#endif
{
	var ret = "";
	if (key == null || key == "")return ret;
	if (val == null || val == "")key="";
	ret = "?";
	ret += key+"=";
	ret += encodeURIComponent(val);
	return ret;
}

#ifdef JAVASCRIPT
appendQueryValue = function(key, val)
#else
var appendQueryValue(var key, var val)
#endif
{
	var ret = "";
	if (key == null || key == "")return ret;
	if (val == null || val == "")key = "";
	ret = "&";
	ret += key+"=";
	ret += encodeURIComponent(val);
	return ret;
}

//
// main
//


#ifndef JAVASCRIPT
void main_main(){
#endif
	var ret, str;

	println("Hello JavaScript include test!");

	strarray_test();

	ret = createQueryValue("aaaa", "qq qq");
	ret += appendQueryValue("ccc", ".kl;:@09-^^@:");
	println(ret);
	ret = getQueryValue("?aaa=f++++%35&bbb=&ccc", "aaa");
	println(ret);


	println(myName());
	str = "1:";
	ret = getQuery();
	str += ret;
	println(str);
	if (ret == null || ret == ""){
		setQuery("?qqq=555");
		jumpLocation("main.html");
	}
	println(test);
	println("test end");


#ifndef JAVASCRIPT
}
#endif



#ifdef JAVASCRIPT 
jQuery().ready(function()
#else
void jquery_ready()
#endif
{
	println("jquery.read()");
}
#ifdef JAVASCRIPT 
);
#endif


#ifndef JAVASCRIPT
int main(int argc,char* argv[])
{
	main_main();
	jquery_ready();
	return 0;
}
#endif



