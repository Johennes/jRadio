jRadio
======

An internet radio player for the Linux desktop

jRadio is an easy to use internet radio player for your Linux desktop. The
application is written in C++ using Gtkmm and (among others) includes the
following features

* robust playback of web radio streams using VLC
* meta data support
* playlist history
* customizable station icons

### How to Install

To compile the sources use the following commands from the root directory.

``` shell
autoreconf -vif
./configure [--prefix=PREFIX]
make
```

Afterwards you can install the application into the specified PREFIX directory
by using the command

``` shell
make install
```
