#!/bin/sh

libtoolize --automake
aclocal
autoconf
automake --add-missing
./configure
gmake