#!/bin/sh
VERSION=0.1.0

rm *.gem *.tar.bz2 2> /dev/null
rm -rf doc

rdoc -w 4 -SHN -f darkfish -m README --title 'bayon/Ruby - Ruby bindings for bayon.' README

mkdir work
cp -r * work 2> /dev/null
cd work

tar jcf bayon-${VERSION}.tar.bz2 --exclude=.svn README *.gemspec ext doc
gem build bayon.gemspec
#gem build bayon-mswin32.gemspec
#cp bayon-${VERSION}-x86-mswin32.gem bayon-${VERSION}-mswin32.gem

#rm -rf lib
#mv lib1.9 lib
#gem build bayon1.9-mswin32.gemspec
cp *.gem *.tar.bz2 ..
cd ..

rm -rf work
