#ifndef JAVASCRIPT
#include <stdio.h>
#include "variant.h"
#include "js_md5.h"
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
md5_println = function(src)
#else
void md5_println(var src)
#endif
{
#ifdef JAVASCRIPT
	alert(src);
#else
	printf("%s\n", ((std::string)src).c_str());
#endif
}



var this_au32_state;
var this_au32_buffer;
var this_n_blocks;

#ifdef JAVASCRIPT
md5_init = function()
#else
void md5_init()
#endif
{
	this_au32_state = new Array();
	this_au32_buffer = new Array();
	this_n_blocks = 0;
	this_au32_state[0] = 0x67452301;
	this_au32_state[1] = 0xefcdab89;
	this_au32_state[2] = 0x98badcfe;
	this_au32_state[3] = 0x10325476;

}

#ifdef JAVASCRIPT
md5_rshift = function(a,b)
#else
var md5_rshift(var a,var b)
#endif
{
	if (b < 1)return a;
	if (b == 1)return (a >> 1)& 0x7fffffff;
	a=(a >> 1) & 0x7fffffff;
	a >>= (b - 1);
	return a;
}



#ifdef JAVASCRIPT
md5_transform = function()
#else
void md5_transform()
#endif
{
	var a, b, c, d;
	var x00, x01, x02, x03, x04, x05, x06, x07, x08, x09, x10, x11, x12, x13, x14, x15;
	var n_idx = 0;
	for (var i = 0; i<this_n_blocks; i++) {
		a = this_au32_state[0];
		b = this_au32_state[1];
		c = this_au32_state[2];
		d = this_au32_state[3];

		x00 = this_au32_buffer[n_idx];
		n_idx++;
		x01 = this_au32_buffer[n_idx];
		n_idx++;
		x02 = this_au32_buffer[n_idx];
		n_idx++;
		x03 = this_au32_buffer[n_idx];
		n_idx++;
		x04 = this_au32_buffer[n_idx];
		n_idx++;
		x05 = this_au32_buffer[n_idx];
		n_idx++;
		x06 = this_au32_buffer[n_idx];
		n_idx++;
		x07 = this_au32_buffer[n_idx];
		n_idx++;
		x08 = this_au32_buffer[n_idx];
		n_idx++;
		x09 = this_au32_buffer[n_idx];
		n_idx++;
		x10 = this_au32_buffer[n_idx];
		n_idx++;
		x11 = this_au32_buffer[n_idx];
		n_idx++;
		x12 = this_au32_buffer[n_idx];
		n_idx++;
		x13 = this_au32_buffer[n_idx];
		n_idx++;
		x14 = this_au32_buffer[n_idx];
		n_idx++;
		x15 = this_au32_buffer[n_idx];
		n_idx++;

		// Round 1
		a += ((b & c) | (~b & d)) + x00 + 0xd76aa478;	a = ((a << 7) | md5_rshift(a, 25)) + b;
		d += ((a & b) | (~a & c)) + x01 + 0xe8c7b756;	d = ((d << 12) | md5_rshift(d, 20)) + a;
		c += ((d & a) | (~d & b)) + x02 + 0x242070db;	c = ((c << 17) | md5_rshift(c, 15)) + d;
		b += ((c & d) | (~c & a)) + x03 + 0xc1bdceee;	b = ((b << 22) | md5_rshift(b, 10)) + c;
		a += ((b & c) | (~b & d)) + x04 + 0xf57c0faf;	a = ((a << 7) | md5_rshift(a, 25)) + b;
		d += ((a & b) | (~a & c)) + x05 + 0x4787c62a;	d = ((d << 12) | md5_rshift(d, 20)) + a;
		c += ((d & a) | (~d & b)) + x06 + 0xa8304613;	c = ((c << 17) | md5_rshift(c, 15)) + d;
		b += ((c & d) | (~c & a)) + x07 + 0xfd469501;	b = ((b << 22) | md5_rshift(b, 10)) + c;
		a += ((b & c) | (~b & d)) + x08 + 0x698098d8;	a = ((a << 7) | md5_rshift(a,  25)) + b;
		d += ((a & b) | (~a & c)) + x09 + 0x8b44f7af;	d = ((d << 12) | md5_rshift(d, 20)) + a;
		c += ((d & a) | (~d & b)) + x10 + 0xffff5bb1;	c = ((c << 17) | md5_rshift(c, 15)) + d;
		b += ((c & d) | (~c & a)) + x11 + 0x895cd7be;	b = ((b << 22) | md5_rshift(b, 10)) + c;
		a += ((b & c) | (~b & d)) + x12 + 0x6b901122;	a = ((a << 7) | md5_rshift(a,  25)) + b;
		d += ((a & b) | (~a & c)) + x13 + 0xfd987193;	d = ((d << 12) | md5_rshift(d, 20)) + a;
		c += ((d & a) | (~d & b)) + x14 + 0xa679438e;	c = ((c << 17) | md5_rshift(c, 15)) + d;
		b += ((c & d) | (~c & a)) + x15 + 0x49b40821;	b = ((b << 22) | md5_rshift(b, 10)) + c;
		// Round 2
		a += ((b & d) | (c & ~d)) + x01 + 0xf61e2562;	a = ((a << 5) | md5_rshift(a, 27)) + b;
		d += ((a & c) | (b & ~c)) + x06 + 0xc040b340;	d = ((d << 9) | md5_rshift(d, 23)) + a;
		c += ((d & b) | (a & ~b)) + x11 + 0x265e5a51;	c = ((c << 14) | md5_rshift(c, 18)) + d;
		b += ((c & a) | (d & ~a)) + x00 + 0xe9b6c7aa;	b = ((b << 20) | md5_rshift(b, 12)) + c;
		a += ((b & d) | (c & ~d)) + x05 + 0xd62f105d;	a = ((a << 5) | md5_rshift(a, 27)) + b;
		d += ((a & c) | (b & ~c)) + x10 + 0x2441453;	d = ((d << 9) | md5_rshift(d, 23)) + a;
		c += ((d & b) | (a & ~b)) + x15 + 0xd8a1e681;	c = ((c << 14) | md5_rshift(c, 18)) + d;
		b += ((c & a) | (d & ~a)) + x04 + 0xe7d3fbc8;	b = ((b << 20) | md5_rshift(b, 12)) + c;
		a += ((b & d) | (c & ~d)) + x09 + 0x21e1cde6;	a = ((a << 5) | md5_rshift(a, 27)) + b;
		d += ((a & c) | (b & ~c)) + x14 + 0xc33707d6;	d = ((d << 9) | md5_rshift(d, 23)) + a;
		c += ((d & b) | (a & ~b)) + x03 + 0xf4d50d87;	c = ((c << 14) | md5_rshift(c, 18)) + d;
		b += ((c & a) | (d & ~a)) + x08 + 0x455a14ed;	b = ((b << 20) | md5_rshift(b, 12)) + c;
		a += ((b & d) | (c & ~d)) + x13 + 0xa9e3e905;	a = ((a << 5) | md5_rshift(a, 27)) + b;
		d += ((a & c) | (b & ~c)) + x02 + 0xfcefa3f8;	d = ((d << 9) | md5_rshift(d,23)) + a;
		c += ((d & b) | (a & ~b)) + x07 + 0x676f02d9;	c = ((c << 14) | md5_rshift(c,18)) + d;
		b += ((c & a) | (d & ~a)) + x12 + 0x8d2a4c8a;	b = ((b << 20) | md5_rshift(b, 12)) + c;
		// Round 3
		a += (b ^ c ^ d) + x05 + 0xfffa3942;	a = ((a << 4) | md5_rshift(a, 28)) + b;
		d += (a ^ b ^ c) + x08 + 0x8771f681;	d = ((d << 11) | md5_rshift(d, 21)) + a;
		c += (d ^ a ^ b) + x11 + 0x6d9d6122;	c = ((c << 16) | md5_rshift(c, 16)) + d;
		b += (c ^ d ^ a) + x14 + 0xfde5380c;	b = ((b << 23) | md5_rshift(b, 9)) + c;
		a += (b ^ c ^ d) + x01 + 0xa4beea44;	a = ((a << 4) | md5_rshift(a, 28)) + b;
		d += (a ^ b ^ c) + x04 + 0x4bdecfa9;	d = ((d << 11) | md5_rshift(d, 21)) + a;
		c += (d ^ a ^ b) + x07 + 0xf6bb4b60;	c = ((c << 16) | md5_rshift(c, 16)) + d;
		b += (c ^ d ^ a) + x10 + 0xbebfbc70;	b = ((b << 23) | md5_rshift(b, 9)) + c;
		a += (b ^ c ^ d) + x13 + 0x289b7ec6;	a = ((a << 4) | md5_rshift(a, 28)) + b;
		d += (a ^ b ^ c) + x00 + 0xeaa127fa;	d = ((d << 11) | md5_rshift(d, 21)) + a;
		c += (d ^ a ^ b) + x03 + 0xd4ef3085;	c = ((c << 16) | md5_rshift(c, 16)) + d;
		b += (c ^ d ^ a) + x06 + 0x4881d05;	b = ((b << 23) | md5_rshift(b, 9)) + c;
		a += (b ^ c ^ d) + x09 + 0xd9d4d039;	a = ((a << 4) | md5_rshift(a, 28)) + b;
		d += (a ^ b ^ c) + x12 + 0xe6db99e5;	d = ((d << 11) | md5_rshift(d, 21)) + a;
		c += (d ^ a ^ b) + x15 + 0x1fa27cf8;	c = ((c << 16) | md5_rshift(c, 16)) + d;
		b += (c ^ d ^ a) + x02 + 0xc4ac5665;	b = ((b << 23) | md5_rshift(b, 9)) + c;
		// Round 4
		a += (c ^ (b | ~d)) + x00 + 0xf4292244;	a = ((a << 6) | md5_rshift(a, 26)) + b;
		d += (b ^ (a | ~c)) + x07 + 0x432aff97;	d = ((d << 10) | md5_rshift(d, 22)) + a;
		c += (a ^ (d | ~b)) + x14 + 0xab9423a7;	c = ((c << 15) | md5_rshift(c, 17)) + d;
		b += (d ^ (c | ~a)) + x05 + 0xfc93a039;	b = ((b << 21) | md5_rshift(b, 11)) + c;
		a += (c ^ (b | ~d)) + x12 + 0x655b59c3;	a = ((a << 6) | md5_rshift(a, 26)) + b;
		d += (b ^ (a | ~c)) + x03 + 0x8f0ccc92;	d = ((d << 10) | md5_rshift(d, 22)) + a;
		c += (a ^ (d | ~b)) + x10 + 0xffeff47d;	c = ((c << 15) | md5_rshift(c, 17)) + d;
		b += (d ^ (c | ~a)) + x01 + 0x85845dd1;	b = ((b << 21) | md5_rshift(b, 11)) + c;
		a += (c ^ (b | ~d)) + x08 + 0x6fa87e4f;	a = ((a << 6) | md5_rshift(a, 26)) + b;
		d += (b ^ (a | ~c)) + x15 + 0xfe2ce6e0;	d = ((d << 10) | md5_rshift(d, 22)) + a;
		c += (a ^ (d | ~b)) + x06 + 0xa3014314;	c = ((c << 15) | md5_rshift(c, 17)) + d;
		b += (d ^ (c | ~a)) + x13 + 0x4e0811a1;	b = ((b << 21) | md5_rshift(b, 11)) + c;
		a += (c ^ (b | ~d)) + x04 + 0xf7537e82;	a = ((a << 6) | md5_rshift(a, 26)) + b;
		d += (b ^ (a | ~c)) + x11 + 0xbd3af235;	d = ((d << 10) | md5_rshift(d, 22)) + a;
		c += (a ^ (d | ~b)) + x02 + 0x2ad7d2bb;	c = ((c << 15) | md5_rshift(c, 17)) + d;
		b += (d ^ (c | ~a)) + x09 + 0xeb86d391;	b = ((b << 21) | md5_rshift(b,11)) + c;

		this_au32_state[0] += a;
		this_au32_state[1] += b;
		this_au32_state[2] += c;
		this_au32_state[3] += d;
	}

}

#ifdef JAVASCRIPT
md5_setstring= function(str)
#else
var md5_setstring(var str)
#endif
{
	var ret = 0;
	this_au32_buffer = new Array();
	var n_idx = 0, n_shift = 0, n_code;
	var n_len;
#ifdef JAVASCRIPT
	n_len= str.length;
#else
	n_len = str.length();
#endif
	for (var i = 0; i<n_len; i++) {
		n_code = str.charCodeAt(i);
		this_au32_buffer[n_idx] |= (n_code << n_shift);
		(n_shift == 24) ? (n_idx++, n_shift = 0) : n_shift += 8;
	}
	ret=n_idx * 4 + n_shift / 8;
	return ret;
}



#ifdef JAVASCRIPT
md5_padding = function(n_length)
#else
void md5_padding(var n_length)
#endif
{
	var n_mod = n_length % 4;
	var n_idx = (n_length - n_mod) / 4;
	var a;
	a = 0x80;
	a<<= (n_mod * 8);
	this_au32_buffer[n_idx] |= a;
	n_idx++;
	while (n_idx % 16 != 14) {
		this_au32_buffer[n_idx] = 0;
		n_idx++;
	}
	var n_bit_len = n_length * 8;
	this_au32_buffer[n_idx] = n_bit_len;
	n_idx++;
	this_au32_buffer[n_idx] = 0;// Math.floor(n_bit_len / 4294967296);
	n_idx++;
	this_n_blocks = n_idx / 16;
}


#ifdef JAVASCRIPT
md5_result = function()
#else
var md5_result()
#endif
{
	var ret;
	var i,t,a,b;

	ret=new Array();

	for(i=0;i<4;i++){
		t=this_au32_state[i];
		ret[i * 4 + 0] = t & 255;
		ret[i * 4 + 1] = (t>>8) & 255;
		ret[i * 4 + 2] = (t>>16) & 255;
		ret[i * 4 + 3] = (t>>24) & 255;
	}
	return ret;
}


#ifdef JAVASCRIPT
md5_md5 = function(str)
#else
var md5_md5(var str)
#endif
{
	var n_bytes = 0;
	var ret;
	md5_init();
	n_bytes = md5_setstring(str);
	md5_padding(n_bytes);
	md5_transform();
	ret = md5_result();
	return ret;
}






#ifdef JAVASCRIPT
md5_test = function()
#else
void md5_test()
#endif
{
	//var s_str = "hoge--";
	var s_str = "hoge--12345678901234567890";
	var a, b, i;

	a=md5_md5(s_str);
	for (i = 0; i < 16; i++){
		b = a[i];
		md5_println(b);
	}
}

