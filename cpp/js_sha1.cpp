#ifndef JAVASCRIPT
#include <stdio.h>
#include "variant.h"
#include "js_sha1.h"
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
sha1_println = function(src)
#else
void sha1_println(var src)
#endif
{
#ifdef JAVASCRIPT
	alert(src);
#else
	printf("%s\n", ((std::string)src).c_str());
#endif
}



var blockLen;
var state;
var sttLen;
var K;

#ifdef JAVASCRIPT
sha1_init = function()
#else
void sha1_init()
#endif
{
	blockLen = 64;
	sttLen = 5;
	state = new Array();
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
	state[4] = 0xc3d2e1f0;
	K = new Array();
	K[0] = 0x5a827999;
	K[1] = 0x6ed9eba1;
	K[2] = 0x8f1bbcdc;
	K[3] = 0xca62c1d6;
}

#ifdef JAVASCRIPT
sha1_rshift = function(a,b)
#else
var sha1_rshift(var a,var b)
#endif
{
	if (b < 1)return a;
	if (b == 1)return (a >> 1)& 0x7fffffff;
	a=(a >> 1) & 0x7fffffff;
	a >>= (b - 1);
	return a;
}



#ifdef JAVASCRIPT
sha1_rotl = function(v, s)
#else
var sha1_rotl(var v, var s)
#endif
{
	var ret=0;
	var t=32;
	ret = (v << s) | sha1_rshift(v,t - s);
	return ret;
};



#ifdef JAVASCRIPT
sha1_round = function(_blk)
#else
var sha1_round(var _blk)
#endif
{
	var stt = new Array();
	var tmpS = new Array();
	var i, j, tmp,ii,n,len;
	var x = new Array();
	var ret;

	for (j = 0; j<sttLen; j++) stt[j] = state[j];
#ifdef JAVASCRIPT
	len = _blk.length;
#else
	len = _blk.length();
#endif
	for (i = 0; i<len; i += blockLen)
	{
		for (j = 0; j<sttLen; j++) tmpS[j] = stt[j];

		//x = toBigEndian32(_blk.slice(i, i + blockLen));
		for (ii = i,n=0; n < blockLen; ii+=4,n++){

			x[n] = (_blk[ii] << 24) | (_blk[ii + 1] << 16) | (_blk[ii + 2] << 8) | _blk[ii + 3];
		}

		for (j = 16; j<80; j++)
			x[j] = sha1_rotl(x[j - 3] ^ x[j - 8] ^ x[j - 14] ^ x[j - 16], 1);

		for (j = 0; j<80; j++)
		{
			if (j<20)
				tmp = ((stt[1] & stt[2]) ^ (~stt[1] & stt[3])) + K[0];
			else if (j<40)
				tmp = (stt[1] ^ stt[2] ^ stt[3]) + K[1];
			else if (j<60)
				tmp = ((stt[1] & stt[2]) ^ (stt[1] & stt[3]) ^ (stt[2] & stt[3])) + K[2];
			else
				tmp = (stt[1] ^ stt[2] ^ stt[3]) + K[3];

			tmp += sha1_rotl(stt[0], 5) + x[j] + stt[4];
			stt[4] = stt[3];
			stt[3] = stt[2];
			stt[2] = sha1_rotl(stt[1], 30);
			stt[1] = stt[0];
			stt[0] = tmp;
		}
		for (j = 0; j<sttLen; j++) stt[j] += tmpS[j];
	}

	//return fromBigEndian32(stt);
#ifdef JAVASCRIPT
	len = stt.length;
#else
	len = stt.length();
#endif
	ret = new Array();

	for (n = i = 0; i<len; i++)
	{
		ret[n] = (stt[i] >> 24) & 0xff;
		n++;
		ret[n] = (stt[i] >> 16) & 0xff;
		n++;
		ret[n] = (stt[i] >> 8) & 0xff;
		n++;
		ret[n] = stt[i] & 0xff;
		n++;
	}


	return ret;
}



#ifdef JAVASCRIPT
sha1_padding = function(_datz)
#else
var sha1_padding(var _datz)
#endif
{
	var datLen, n;
#ifdef JAVASCRIPT
	datLen = _datz.length;
#else
	datLen = _datz.length();
#endif
	n = datLen;

	_datz[n] = 0x80;
	n++;

	while (n% blockLen != 56){
		_datz[n] = 0;
		n++;
	}
	datLen *= 8;

	//return _datz.concat(0, 0, 0, 0, fromBigEndian32([datLen]) );
	_datz[n] = 0;
	n++;
	_datz[n] = 0;
	n++;
	_datz[n] = 0;
	n++;
	_datz[n] = 0;
	n++;

	_datz[n] = (datLen>>24) & 255;
	n++;
	_datz[n] = (datLen >> 16) & 255;
	n++;
	_datz[n] = (datLen >> 8) & 255;
	n++;
	_datz[n] = datLen & 255;
	n++;

	return _datz;


}

#ifdef JAVASCRIPT
sha1_unpack = function(_dat)
#else
var sha1_unpack(var _dat)
#endif
{
	var i, n, c, tmp,len;
	tmp = new Array();
#ifdef JAVASCRIPT
	len=_dat.length;
#else
	len = _dat.length();
#endif

	for (n =0, i = 0; i<len; i++)
	{
		c = _dat.charCodeAt(i);
		tmp[n] = c;
		n++;
	}
	return tmp;
}



#ifdef JAVASCRIPT
sha1_sha1 = function(str)
#else
var sha1_sha1(var str)
#endif
{
	var a;
	sha1_init();
	a = sha1_unpack(str);
	a = sha1_padding(a);
	a = sha1_round(a);
	return a;
}




#ifdef JAVASCRIPT
sha1_test = function()
#else
void sha1_test()
#endif
{
	var s_str = "hoge--hoge--hoge--hoge--hoge--hoge--hoge--0123456789";
	var a,b,i;
	
	a = sha1_sha1(s_str);

#ifdef JAVASCRIPT
	b = a.length;
#else
	b = a.length();
#endif
	sha1_println(b);
	sha1_println("");


	for (i = 0; i < 20; i++){
		b = a[i];
		sha1_println(b);
	}
}

