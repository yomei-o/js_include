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


var base64list = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

#ifdef JAVASCRIPT
base64_println = function(src)
#else
void base64_println(var src)
#endif
{
#ifdef JAVASCRIPT
	alert(src);
#else
	printf("%s\n", ((std::string)src).c_str());
#endif
}


#ifdef JAVASCRIPT
var base64_encode = function(s)
#else
var base64_encode(var s)
#endif
{
	var t = "";
	var p = -6, a = 0, i = 0, v = 0, c,len,u;
#ifdef JAVASCRIPT
	len = s.length;
#else
	len = s.length();
#endif
	while ((i < len) || (p > -6)) {
		if (p < 0) {
			if (i < len) {
				c = s[i];
				i++;
				v += 8;
			}
			else {
				c = 0;
			}
			a = ((a & 255) << 8) | (c & 255);
			p += 8;
		}
		u = (v > 0) ? ((a >> p) & 63) : 64;
		t += base64list.charAt(u);
		p -= 6;
		v -= 6;
	}
	return t;
}

#ifdef JAVASCRIPT
var base64_decode = function(s)
#else
var base64_decode(var s)
#endif
{
	var t , p = -8, a = 0, c, d,len,j=0;

	t = new Array();

#ifdef JAVASCRIPT
	len = s.length;
#else
	len = s.length();
#endif

	for (var i = 0; i < len; i++) {
		if ((c = base64list.indexOf(s.charAt(i))) < 0)
			continue;
		a = (a << 6) | (c & 63);
		if ((p += 6) >= 0) {
			d = (a >> p) & 255;
			if (c != 64)
			t[j] = d;
			j++;
			a &= 63;
			p -= 8;
		}
	}
	return t;

}

#if 0

Base64a.encode([97, 98, 99, 100]); //=> "YWJjZA=="
Base64a.decode("YWJjZA=="); //=> [97, 98, 99, 100]
ArrayBuffer, Blob(File)Ç»Ç«Ç©ÇÁïœä∑Ç∑ÇÈèÍçá

#endif















