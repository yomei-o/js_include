//// JavaScript include library  Rev 0.05 ----BEGIN----
if(typeof include=="undefined"){var t,q="__QUERY__";include_list={};include=function(s){if(typeof($)!="undefined" && s.indexOf("jquery-")==0)return;if(s.indexOf(q)!=-1){
if(typeof sessionStorage!="undefined" && (t=sessionStorage.getItem(q)))s=s.replace(q,t);
else s=s.replace(q,location.search);}
var xhr=null;if(include_list[s])return;include_list[s]=1;if (window.XMLHttpRequest)xhr=new XMLHttpRequest();
else if(window.ActiveXObject)try{xhr=new ActiveXObject("Msxml2.XMLHTTP");}catch(e){xhr=new ActiveXObject("Microsoft.XMLHTTP");}
try{xhr.open("GET",s,false);xhr.send("");(0,eval)(xhr.responseText);}catch(e){}}}declare_var_begin=declare_var_end=function(){};
//// JavaScript include library ----END----
include("json_sample.exe__QUERY__");
include("jquery-1.11.3.js");
include("json_sample.js");
//include("json_sample.exe__QUERY__");
include("js_base64.js");
//
//
//
//
// Debufg print
//
println = function(src)
{
 alert(src);
}
//
// length
//
strlength = function(src)
{
 var ret=0;
 if (src == null || src == "")return ret;
 ret=src.length;
 return ret;
}
//
// QUERY
//
var f_set_query = 0;
getQuery = function()
{
 var ret;
 ret= "";
 if (location.search){
  ret = location.search;
  return ret;
 }
 if (typeof(sessionStorage) != "undefined"){
  ret = sessionStorage.getItem("__QUERY__");
  if (ret!=null)return ret;
 }
 return ret;
}
setQuery = function(src)
{
 if (src == null || src.indexOf("?") != 0)return;
 f_set_query = 1;
 if (typeof(sessionStorage) != "undefined"){
  sessionStorage.setItem("__QUERY__",src);
  return;
 }
 __QUERY__ = src;
}
clearQuery = function()
{
 f_set_query = 0;
 if (typeof(sessionStorage) != "undefined"){
  sessionStorage.removeItem("__QUERY__");
  return;
 }
 __QUERY__ = null;
}
jumpLocation = function(src)
{
 if(src==null)return;
 if(f_set_query==0)clearQuery();
 if(typeof(__QUERY__)!="undefined")src+=__QUERY__;
 location.href = src;
}
myName = function()
{
 var ret="";
 var idx = -1;
 ret = location.href;
 idx = ret.lastIndexOf("/");
 if (idx != -1)ret = ret.substring(idx + 1);
 idx = ret.lastIndexOf("\\");
 if (idx != -1)ret = ret.substring(idx + 1);
 idx = ret.lastIndexOf("?");
 if (idx != -1)ret = ret.substring(0,idx);
 return ret;
}
getQueryValue = function(src,key)
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
createQueryValue = function(key,val)
{
 var ret = "";
 if (key == null || key == "")return ret;
 if (val == null || val == "")key="";
 ret = "?";
 ret += key+"=";
 ret += encodeURIComponent(val);
 return ret;
}
appendQueryValue = function(key, val)
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
// query debug
//
getQueryLocationSearch = function()
{
 var ret=null;
 if (location.search){
  ret = location.search;
 }
 return ret;
}
getQuerySessionStorage = function()
{
 var ret=null;
 if (typeof(sessionStorage) != "undefined"){
  ret = sessionStorage.getItem("__QUERY__");
 }
 return ret;
}
//
// print,mail,open new window 
//
printPage = function()
{
 try{;
 print();
 }catch(e){};
}
openPage = function(src)
{
 // This function should be called from onClick events
 if(f_set_query==0)clearQuery();
 try{;
 window.open(src);
 }catch(e){};
}
mailTo = function(addr,subject,body)
{
 if (f_set_query == 0)clearQuery();
 try{;
 location.href = 'mailto:' + addr + '?subject=' + subject + '&body=' + body;
 }catch(e){};
}
//
// main
//
 var ret=null, str;
 println("Hello JavaScript include test!");
 //str = "querySessionStorage ==> ";
 //ret = getQuerySessionStorage();
 //str += ret;
 //println(str);
 //str = "queryLocationSearch ==> ";
 //ret = getQueryLocationSearch();
 //str += ret;
 //println(str);
 str = "query ==> ";
 ret = getQuery();
 str += ret;
 println(str);
 if (ret == null || ret == ""){
  setQuery("?qqq=555");
  jumpLocation("main.html");
 }
 else{
  println("open");
  openPage("sub.html");
 }
jQuery().ready(function()
{
 println("jquery.read()");
}
);
