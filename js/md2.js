
/*  /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	charset= shift_jis
	
    [RFC 1319] The MD2 Message-Digest Algorithm
    http://www.ietf.org/rfc/rfc1319.txt

    LastModified : 2006-11/14
    
    Written by kerry
    http://user1.matsumoto.ne.jp/~goma/

    ����u���E�U :: IE4+ , NN4.06+ , Gecko , Opera6
    
    ----------------------------------------------------------------
    
    Usage
    
    // �Ԃ�l�� 16�i���œ���
    md2hash = md2.hex( data );
	
	// �Ԃ�l���o�C�i���œ���
    md2bin = md2.bin( data );
    
    // �Ԃ�l��10�i���̔z��œ���
    md2decs = md2.dec( data );
    
    
	* data		-> �n�b�V���l�𓾂����f�[�^
				data �̓A���p�b�N�ς݂̔z��ł��\

	// e.g.
	
	var data_1 = "abc";
	var hash_1 = md2.hex( data_1 );
	var data_2 = new Array(data_1.charCodeAt(0), data_1.charCodeAt(1), data_1.charCodeAt(2));
	var hash_2 = md2.hex( data_2 );
	
	alert( hash_1 === hash_2 ); // true
	
/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/   */



md2 = new function()
{
	var blockSize = 16;
	
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
		datz = addCheckSum(datz);
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
	
	var round = function(_dz)
	{
		var i, t, k, X = [];
		var n = _dz.length/ blockSize;
		
		for (i=0; i<48; i++) X[i] = 0;
		
		for (i=0; i<n; i++)
		{
			for (j=0; j<=0xf; j++)
			{
				X[16+ j] = _dz[i* 16+ j];
				X[32+ j] = X[16+ j]^ X[j];
			}

			for (t=j=0; j<18; j++)
			{
				for (k=0; k<48; k++)
					t = X[k] ^= S[t];

				t = (t+ j) % 256;
			}
		}
		return X.slice(0, 16);
	}


	var addCheckSum = function(_dz)
	{
		var i, j, c;
		var C = [];
		var L = 0;
		var n = _dz.length/ blockSize;

		for (i=0; i<=0xf; i++) C[i] = 0;
		
		for (i=0; i<n; i++)
		{
			for (j=0; j<=0xf; j++)
			{
				c = _dz[i* 16+ j];
				C[j] ^= S[c ^ L];
				L = C[j];
			}
		}
		return _dz.concat(C);
	}

	var paddingData = function(_dz)
	{
		var n  	= _dz.length;
		var md 	= blockSize- (n % blockSize);
		for (var i=0; i<md; i++, n++) _dz[n] = md;
		return _dz;
	}

	var toHex = function(_decz)
	{
		var i, hex = "";

		for (i=0; i<_decz.length; i++)
			hex += (_decz[i]>0xf?"":"0")+ _decz[i].toString(16);
		return hex;
	}

	var unpack = function(_dat)
	{
		var i, n, tmp = [];

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
        var i, dat = "";
        for (i in _ary) dat += String.fromCharCode(_ary[i]);
        return dat;
    }

	
	var S = [
	 41,  46,  67, 201, 162, 216, 124,   1,  61,  54,  84, 161, 236, 240,   6,  19, 
	 98, 167,   5, 243, 192, 199, 115, 140, 152, 147,  43, 217, 188,  76, 130, 202, 
	 30, 155,  87,  60, 253, 212, 224,  22, 103,  66, 111,  24, 138,  23, 229,  18, 
	190,  78, 196, 214, 218, 158, 222,  73, 160, 251, 245, 142, 187,  47, 238, 122, 
	169, 104, 121, 145,  21, 178,   7,  63, 148, 194,  16, 137,  11,  34,  95,  33, 
	128, 127,  93, 154,  90, 144,  50,  39,  53,  62, 204, 231, 191, 247, 151,   3, 
	255,  25,  48, 179,  72, 165, 181, 209, 215,  94, 146,  42, 172,  86, 170, 198, 
	 79, 184,  56, 210, 150, 164, 125, 182, 118, 252, 107, 226, 156, 116,   4, 241, 
	 69, 157, 112,  89, 100, 113, 135,  32, 134,  91, 207, 101, 230,  45, 168,   2, 
	 27,  96,  37, 173, 174, 176, 185, 246,  28,  70,  97, 105,  52,  64, 126,  15, 
	 85,  71, 163,  35, 221,  81, 175,  58, 195,  92, 249, 206, 186, 197, 234,  38, 
	 44,  83,  13, 110, 133,  40, 132,   9, 211, 223, 205, 244,  65, 129,  77,  82, 
	106, 220,  55, 200, 108, 193, 171, 250,  36, 225, 123,   8,  12, 189, 177,  74, 
	120, 136, 149, 139, 227,  99, 232, 109, 233, 203, 213, 254,  59,   0,  29,  57, 
	242, 239, 183,  14, 102,  88, 208, 228, 166, 119, 114, 248, 235, 117,  75,  10, 
	 49,  68,  80, 180, 143, 237,  31,  26, 219, 153, 141,  51, 159,  17, 131,  20
	];

}





