
/*  /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	charset= shift_jis
	
    +++++ RIPEMD-160 +++++
    http://www.esat.kuleuven.ac.be/~bosselae/ripemd160.html


    LastModified : 2006-11/14
    
    Written by kerry
    http://user1.matsumoto.ne.jp/~goma/

    動作ブラウザ :: IE4+ , NN4.06+ , Gecko , Opera6
    
    ----------------------------------------------------------------
    
    Usage
    
    // 返り値を 16進数で得る
    rmdhash = ripemd.hex( data [, bitLen] );
	
	// 返り値をバイナリで得る
    rmdbin = ripemd.bin( data [, bitLen] );
    
    // 返り値を10進数の配列で得る
    rmddecs = ripemd.dec( data [, bitLen] );
    
    
	* data		-> ハッシュ値を得たいデータ
				data はアンパック済みの配列でも可能
	* bitLen	-> 128, 160, 256, 320 のいずれか。省略時は 160
					128 -> RIPEMD-128
					160 -> RIPEMD-160
					256 -> RIPEMD-256
					320 -> RIPEMD-320
	
	// e.g.
	
	var data_1 = "abc";
	var hash_1 = ripemd.hex( data_1 );
	var data_2 = new Array(data_1.charCodeAt(0), data_1.charCodeAt(1), data_1.charCodeAt(2));
	var hash_2 = ripemd.hex( data_2 );
	
	alert( hash_1 === hash_2 ); // true
	
/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/   */


ripemd = new function()
{
	var blockLen = 64;

	var state = {
		rmd128: [ 0x67452301 , 0xEFCDAB89 , 0x98BADCFE , 0x10325476 ],
		rmd256: [ 0x67452301 , 0xEFCDAB89 , 0x98BADCFE , 0x10325476 ,
          		  0x76543210 , 0xFEDCBA98 , 0x89ABCDEF , 0x01234567 ],
		rmd160: [ 0x67452301 , 0xEFCDAB89 , 0x98BADCFE , 0x10325476 , 0xC3D2E1F0 ],
        rmd320: [ 0x67452301 , 0xEFCDAB89 , 0x98BADCFE , 0x10325476 , 0xC3D2E1F0 ,
        		  0x76543210 , 0xFEDCBA98 , 0x89ABCDEF , 0x01234567 , 0x3C2D1E0F ]
	};

	this.hex = function(_data, _bitLen)
	{
		return toHex( getMD(_data, _bitLen) );
	}
	
	this.dec = function(_data, _bitLen)
	{
		return getMD(_data, _bitLen);
	}
	
	this.bin = function(_data, _bitLen)
	{
		return pack( getMD(_data, _bitLen) );
	}
	
	var getMD = function(_data, _bitLen)
	{
		var datz = [];
		if (isAry(_data)) datz = _data;
		else if (isStr(_data)) datz = unpack(_data);
		else "unknown type";
		datz = paddingData(datz);
		return round(datz, _bitLen);
	}
    
    var isAry = function(_ary)
	{
		return _ary && _ary.constructor === [].constructor;
	}
	var isStr = function(_str)
	{
		return typeof(_str) == typeof("string");
	}

    var rotl = function(_v, _s) { return (_v << _s) | (_v >>> (32 - _s)) };


	var k = function(_x)
	{  
	    if 		(_x < 16) { return 0 }
	    else if (_x < 32) { return 0x5A827999 }
	    else if (_x < 48) { return 0x6ED9EBA1 }
	    else if (_x < 64) { return 0x8F1BBCDC }
	    else if (_x < 80) { return 0xA953FD4E }
	}

	var k2 = function(_x, _flag)
	{  
	    if 		(_x < 16) { return 0x50A28BE6 }
	    else if (_x < 32) { return 0x5C4DD124 }
	    else if (_x < 48) { return 0x6D703EF3 }
	    else if (_x < 64) { return _flag ? 0x7A6D76E9 : 0 }
	    else if (_x < 80) { return 0 }
	}


	var f = function(_a, _b, _c, _x)
	{
	    if 		(_x < 16) { return _a ^  _b  ^  _c  		}
	    else if (_x < 32) { return (_a &  _b) | (~_a & _c)	}
	    else if (_x < 48) { return (_a | ~_b) ^  _c        	}
	    else if (_x < 64) { return (_a &  _c) | (_b & ~_c)	}
	    else if (_x < 80) { return  _a ^ (_b  | ~_c)     	}
	}
	
	
	var round = function(_blk, _bitLen)
	{
		var stt = [];
		var rmdF = new Function();
		
		if (_bitLen == 128)
		{
			aryCopy(stt, state.rmd128);
			rmdF = round128;
		}
		else if (_bitLen == 256)
		{
			aryCopy(stt, state.rmd256);
			rmdF = round256;
		}
		else if (_bitLen == 320)
		{
			aryCopy(stt, state.rmd320);
			rmdF = round320;
		}
		else // if (_bitLen == 160)
		{
			aryCopy(stt, state.rmd160);
			rmdF = round160;
		}
		
		for (var i=0; i<_blk.length; i+=blockLen)
			stt = rmdF( toLittleEndian32(_blk.slice(i, i+ blockLen)), stt);

		return fromLittleEndian32(stt);
	}
	
	var aryCopy = function(_ary1 /* コピ先 */, _ary2 /* コピ元 */)
	{
		for (var i=0; i<_ary2.length; i++)
			_ary1[i] = _ary2[i];
	}
	
	var swpVal = function(_ary1, _ary2, _i) 
	{
		var t = _ary1[_i];
		_ary1[_i] = _ary2[_i];
		_ary2[_i] = t;
	}
	
	var round128 = function(_blk, _stt)
	{
		var tmp;
		var tmpStt1 = [];
		var tmpStt2 = [];
		aryCopy(tmpStt1, _stt);
		aryCopy(tmpStt2, _stt);

		for (var i=0; i<64; i++)
		{
	        tmp = tmpStt1[0] + f(tmpStt1[1], tmpStt1[2], tmpStt1[3], i) + _blk[ R[i] ] + k(i);
			tmpStt1 = [tmpStt1[3], rotl(tmp, S[i]), tmpStt1[1], tmpStt1[2]];

	        tmp = tmpStt2[0] + f(tmpStt2[1], tmpStt2[2], tmpStt2[3], 63-i) + _blk[ R2[i] ] + k2(i);
	        tmpStt2 = [tmpStt2[3], rotl(tmp, S2[i]), tmpStt2[1], tmpStt2[2]];
		}

	    return [_stt[1] + tmpStt1[2] + tmpStt2[3],
	    		_stt[2] + tmpStt1[3] + tmpStt2[0],
	    		_stt[3] + tmpStt1[0] + tmpStt2[1],
	    		_stt[0] + tmpStt1[1] + tmpStt2[2]
	    		];

	}
	
	var round256 = function(_blk, _stt)
	{
		var i, tmp;
		var tmpStt1 = _stt.slice(0, 4);
		var tmpStt2 = _stt.slice(4);

	    for (i=0; i<64; i++)
	    {
	        tmp = tmpStt1[0] + f(tmpStt1[1], tmpStt1[2], tmpStt1[3], i)+ _blk[ R[i] ] + k(i);
			tmpStt1 = [tmpStt1[3], rotl(tmp, S[i]), tmpStt1[1], tmpStt1[2]];

	        tmp = tmpStt2[0] + f(tmpStt2[1], tmpStt2[2], tmpStt2[3], 63-i)+ _blk[ R2[i] ] + k2(i);
	        tmpStt2 = [tmpStt2[3], rotl(tmp, S2[i]), tmpStt2[1], tmpStt2[2]];
	        
			switch (i) {
				case 15 : swpVal(tmpStt1, tmpStt2, 0); break;
				case 31 : swpVal(tmpStt1, tmpStt2, 1); break;
				case 47 : swpVal(tmpStt1, tmpStt2, 2); break;
				case 63 : swpVal(tmpStt1, tmpStt2, 3); break;
			}
	    }
	    return [
	    	_stt[0] + tmpStt1[0], 	_stt[1] + tmpStt1[1],
	    	_stt[2] + tmpStt1[2], 	_stt[3] + tmpStt1[3],
	    	_stt[4] + tmpStt2[0], 	_stt[5] + tmpStt2[1],
	    	_stt[6] + tmpStt2[2], 	_stt[7] + tmpStt2[3]
	    ];
	}
	
	var round160 = function(_blk, _stt)
	{
		var i, tmp;
		var tmpStt1 = [];
		var tmpStt2 = [];
		aryCopy(tmpStt1, _stt);
		aryCopy(tmpStt2, _stt);

	    for (i=0; i<80; i++)
	    {
	        tmp = tmpStt1[0] + f(tmpStt1[1], tmpStt1[2], tmpStt1[3], i) + _blk[ R[i] ] + k(i);
	 		tmpStt1 = [tmpStt1[4], rotl(tmp, S[i])+ tmpStt1[4], tmpStt1[1], 
	 					rotl(tmpStt1[2] , 10), tmpStt1[3], tmpStt1[4]];

	        tmp = tmpStt2[0] + f(tmpStt2[1], tmpStt2[2], tmpStt2[3], 79-i) + _blk[ R2[i] ] + k2(i, 1);
	        tmpStt2 = [tmpStt2[4], rotl(tmp, S2[i])+ tmpStt2[4], tmpStt2[1], 
	 					rotl(tmpStt2[2] , 10), tmpStt2[3], tmpStt2[4]];
	    }
	    
	    return [
	    	_stt[1] + tmpStt1[2] + tmpStt2[3],
	    	_stt[2] + tmpStt1[3] + tmpStt2[4],
	    	_stt[3] + tmpStt1[4] + tmpStt2[0],
	    	_stt[4] + tmpStt1[0] + tmpStt2[1],
	    	_stt[0] + tmpStt1[1] + tmpStt2[2]
	    ];
	}

	var round320 = function(_blk, _stt)
	{
		var i, tmp;
		var tmpStt1 = _stt.slice(0, 5);
		var tmpStt2 = _stt.slice(5);

	    for (i=0; i<80; i++)
	    {
	        tmp = tmpStt1[0] + f(tmpStt1[1], tmpStt1[2], tmpStt1[3], i) + _blk[ R[i] ] + k(i);
			tmpStt1 = [ tmpStt1[4], rotl(tmp, S[i]) + tmpStt1[4], tmpStt1[1],
						rotl(tmpStt1[2] , 10), tmpStt1[3], tmpStt1[4] ];

	        tmp = tmpStt2[0] + f(tmpStt2[1], tmpStt2[2], tmpStt2[3], 79-i) + _blk[ R2[i] ] + k2(i, 1);
	        tmpStt2 = [ tmpStt2[4], rotl(tmp, S2[i]) + tmpStt2[4], tmpStt2[1],
						rotl(tmpStt2[2] , 10), tmpStt2[3], tmpStt2[4] ];
						
			switch (i) {
				case 15 : swpVal(tmpStt1, tmpStt2, 1); break;
				case 31 : swpVal(tmpStt1, tmpStt2, 3); break;
				case 47 : swpVal(tmpStt1, tmpStt2, 0); break;
				case 63 : swpVal(tmpStt1, tmpStt2, 2); break;
				case 79 : swpVal(tmpStt1, tmpStt2, 4); break;
			}
	    }
	    return [
	    	_stt[0] + tmpStt1[0], 	_stt[1] + tmpStt1[1],
	    	_stt[2] + tmpStt1[2], 	_stt[3] + tmpStt1[3],
	    	_stt[4] + tmpStt1[4], 	_stt[5] + tmpStt2[0],
	    	_stt[6] + tmpStt2[1], 	_stt[7] + tmpStt2[2],
	    	_stt[8] + tmpStt2[3], 	_stt[9] + tmpStt2[4]
	    ];
	}
	
	
	var paddingData = function(_datz)
	{
		var datLen = _datz.length;
		var n = datLen;
		_datz[n++] = 0x80;
		while (n% blockLen != 56) _datz[n++] = 0;
		datLen *= 8;
		return _datz.concat(fromLittleEndian32([datLen]), 0, 0, 0, 0 );
	}

	var toHex = function(_decz)
	{
		var i, hex = "";

		for (i=0; i<_decz.length; i++)
			hex += (_decz[i]>0xf?"":"0")+ _decz[i].toString(16);
		return hex;
	}
	
	var fromLittleEndian32 = function(_blk)
	{
		var tmp = [];
		for (n=i=0; i<_blk.length; i++)
		{
			tmp[n++] = _blk[i] & 0xff;
			tmp[n++] = (_blk[i] >>>  8) & 0xff;
			tmp[n++] = (_blk[i] >>> 16) & 0xff;
			tmp[n++] = (_blk[i] >>> 24) & 0xff;
		}
		return tmp;
	}
	
	var toLittleEndian32 = function(_blk)
	{
		var tmp = [];
		var i, n;
		for (n=i=0; i<_blk.length; i+=4, n++)
			tmp[n] = (_blk[i+ 3]<<24) | (_blk[i+ 2]<<16) | (_blk[i+ 1]<<8) | _blk[i];
		return tmp;
	}

	var unpack = function(_dat)
	{
		var i, n, c, tmp = [];

	    for (n=i=0; i<_dat.length; i++) 
	    {
	    	c = _dat.charCodeAt(i);
			if (c <= 0xff) tmp[n++] = c;
			else {
				tmp[n++] = c >>> 8;
				tmp[n++] = c &  0xff;
			}	
	    }
	    return tmp;
	}

	var pack = function(_ary)
    {
        var i, tmp = "";
        for (i in _ary) tmp += String.fromCharCode(_ary[i]);
        return tmp;
    }

    
    var S  = [
        11, 14, 15, 12,  5,  8,  7,  9, 11, 13, 14, 15,  6,  7,  9,  8,
         7,  6,  8, 13, 11,  9,  7, 15,  7, 12, 15,  9, 11,  7, 13, 12,
        11, 13,  6,  7, 14,  9, 13, 15, 14,  8, 13,  6,  5, 12,  7,  5,
        11, 12, 14, 15, 14, 15,  9,  8,  9, 14,  5,  6,  8,  6,  5, 12,
         9, 15,  5, 11,  6,  8, 13, 12,  5, 12, 13, 14, 11,  8,  5,  6,
    ];
    var S2 = [
         8,  9,  9, 11, 13, 15, 15,  5,  7,  7,  8, 11, 14, 14, 12,  6,
         9, 13, 15,  7, 12,  8,  9, 11,  7,  7, 12,  7,  6, 15, 13, 11,
         9,  7, 15, 11,  8,  6,  6, 14, 12, 13,  5, 14, 13, 13,  7,  5,
        15,  5,  8, 11, 14, 14,  6, 14,  6,  9, 12,  9, 12,  5, 15,  8,
         8,  5, 12,  9, 12,  5, 14,  6,  8, 13,  6,  5, 15, 13, 11, 11,
    ];
    
    var R = [
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
        7,  4, 13,  1, 10,  6, 15,  3, 12,  0,  9,  5,  2, 14, 11,  8,
        3, 10, 14,  4,  9, 15,  8,  1,  2,  7,  0,  6, 13, 11,  5, 12,
        1,  9, 11, 10,  0,  8, 12,  4, 13,  3,  7, 15, 14,  5,  6,  2,
        4,  0,  5,  9,  7, 12,  2, 10, 14,  1,  3,  8, 11,  6, 15, 13,
    ];
    var R2 = [
         5, 14,  7, 0, 9,  2, 11,  4, 13,  6, 15,  8,  1, 10,  3, 12,
         6, 11,  3, 7, 0, 13,  5, 10, 14, 15,  8, 12,  4,  9,  1,  2,
        15,  5,  1, 3, 7, 14,  6,  9, 11,  8, 12,  2, 10,  0,  4, 13,
         8,  6,  4, 1, 3, 11, 15,  0,  5, 12,  2, 13,  9,  7, 10, 14,
        12, 15, 10, 4, 1,  5,  8,  7,  6,  2, 13, 14,  0,  3,  9, 11,
    ];

}


