# WWWFS

This project is aiming to create a http(s) based file system, that parses HTML and serves it as a filesystem. 
`<a>` links pointing to other HTML pages are represented as folders, links pointed to other resources are represented as files. Other tags, such as `<img>`, `<video>`, `<audio>` are also parsed and represented as files.

There are other projects that are trying to achieve something simmilar, but these are not quite as awesome:
* http://httpfs.sourceforge.net/ - allows to mount only one file over http
* https://github.com/cyrus-and/httpfs - requires server side php script

# Current state

Currently project serves only two hardcoded files. As of now http(s) client side and parser is missing. Proof of concept was rewritten to support async lowlevel api and now depends only on fuse and qt. Http client will be finished on next free weekend.

~~Currently project is in "proof of concept" state. For now it only allows reading files in a very inefficient way and there is no folder support.~~

TODO:
* HTTP client
* HTML parser
* Symlink support
* Big file support
* create virtual file _index.html for debugging reasons

# Required libs
* libfuse (fuse3)
* qt >= 5.10

# Compiling
```
cd wwwfs
mkdir build
cd build
cmake ../src
make
```

# Using
```
./wwwfs munt_point
```
