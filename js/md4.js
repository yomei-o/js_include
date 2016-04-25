
/*  /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	charset= shift_jis
	
    [RFC 1320] The MD4 Message-Digest Algorithm

    http://www.ietf.org/rfc/rfc1320.txt

    LastModified : 2006-11/14
    
    Written by kerry
    http://user1.matsumoto.ne.jp/~goma/

    動作ブラウザ :: IE4+ , NN4.06+ , Gecko , Opera6
    
    ----------------------------------------------------------------
    
    Usage
    
    // 返り値を 16進数で得る
    md4hash = md4.hex( data );
	
	// 返り値をバイナリで得る
    md4bin = md4.bin( data );
    
    // 返り値を10進数の配列で得る
    md4decs = md4.dec( data );
    
    
	* data		-> ハッシュ値を得たいデータ
				data はアンパック済みの配列でも可能

	// e.g.
	
	var data_1 = "abc";
	var hash_1 = md4.hex( data_1 );
	var data_2 = new Array(data_1.charCodeAt(0), data_1.charCodeAt(1), data_1.charCodeAt(2));
	var hash_2 = md4.hex( data_2 );
	
	alert( hash_1 === hash_2 ); // true
	
/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/   */


md4 = new function()
{
	var blockLen = 64;
	var state = [ 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 ];
	var sttLen = state.length;
	
	this.hex = function(_data)
	{
		return toHex( getMD(_data) );
	}

	this.dec = function(_data)
	{
		return getMD(_data);
	}
	
	this.bin = function(_data)
	{
		return pack( getMD(_data) );
	}
	
	var getMD = function(_data)
	{
		var datz = [];
		if (isAry(_data)) datz = _data;
		else if (isStr(_data)) datz = unpack(_data);
		else "unknown type";
		datz = paddingData(datz);
	
		return round(datz);
	}
    
    var isAry = function(_ary)
	{
		return _ary && _ary.constructor === [].constructor;
	}
	var isStr = function(_str)
	{
		return typeof(_str) == typeof("string");
	}

	var K = [ 	[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15],
				[0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15],
				[0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15]
			];
	var S = [ [3,7,11,19], [3,5,9,13], [3,9,11,15] ];

	var fF	= function(_x, _y, _z) { 
						return (_x & _y) | (~_x & _z) }
	var fG	= function(_x, _y, _z) { 
						return (_x & _y) | (_x & _z) | (_y & _z) }
	var fH	= function(_x, _y, _z) { 
						return _x ^ _y ^ _z }
	var rotL= function(_x, _n)  { 
						return (_x << _n) | (_x >>> (32-_n)) }
	var Fz = [
		function(_a, _k, _s) { 
						_a[0] = rotL(_a[0]+ fF(_a[1], _a[2], _a[3])+ _k, _s) 
		},
		function(_a, _k, _s) { 
						_a[0] = rotL(_a[0]+ fG(_a[1], _a[2], _a[3])+ _k+ 0x5A827999, _s) 
		},
		function(_a, _k, _s) { 
						_a[0] = rotL(_a[0]+ fH(_a[1], _a[2], _a[3])+ _k+ 0x6ED9EBA1, _s) 
		}
	];
	
	var round = function(_blk)
	{
		var stt = [];
		var x 	= [];
		var tmpS= [];
		var i, j, k, x = [];
		for (j=0; j<sttLen; j++) stt[j] = state[j];
		
		for (i=0; i<_blk.length; i+=blockLen)
		{
			for (j=0; j<sttLen; j++) tmpS[j] = stt[j];
			x = toLittleEndian32( _blk.slice(i, i+ blockLen) );

			for (k=0; k<3; k++) for (j=0; j<16; j++)
			{
				Fz[k](stt, x[K[k][j]], S[k][j%4]);
				stt = [stt[3], stt[0], stt[1], stt[2]];
			}		
			for (j=0; j<sttLen; j++) stt[j] += tmpS[j];
		}

		return fromLittleEndian32(stt);
	}

	var paddingData = function(_datz)
	{
		var datLen = _datz.length;
		var n = datLen;
		_datz[n++] = 0x80;
		while (n% blockLen != 56) _datz[n++] = 0;
		datLen *= 8;
		return _datz.concat(
			fromLittleEndian32([datLen]),
			0, 0, 0, 0
		);
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

}

