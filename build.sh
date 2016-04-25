CSRCDIR=cpp
JSRCDIR=out
TOOLDIR=tool
HTMLDIR=html
PACKAGE=com/yomei


FILES="main js_base64 js_md5 js_sha1"

mkdir -p $JSRCDIR

for i in $FILES; do
echo $i
g++ -o $JSRCDIR/$i.js -DJAVASCRIPT -E -C -P $CSRCDIR/$i.cpp
done

cp -f $HTMLDIR/main.html $JSRCDIR/.
cp -f $HTMLDIR/jquery*.js $JSRCDIR/.


echo ------------------------------
echo Build OK!!!!!!!!
echo ------------------------------

