# WWWFS

This project is aiming to create a http(s) based file system, that parses HTML and serves it as a filesystem. 
`<a>` links pointing to other HTML pages are represented as folders, links pointed to other resources are represented as files. Other tags,
such as `<img>`, `<video>`, `<audio>` are also parsed and represented as files.

There are other projects that are trying to achieve something simmilar, but are not quite as awesome:
* http://httpfs.sourceforge.net/ - allows to mount only one file over http
* https://github.com/cyrus-and/httpfs - requires server side php script

# Current state
Currently project is in "proof of concept" state. For now it only allows reading files in a very inefficient way and there is no folder support.

TODO:
* fix naming and spacing
* create virtual file _index.html for debugging reasons
* create data structure for managing FS structure in memmory
* follow links (folders support)

# Compiling
```
cd wwwfs
mkdir build
cd build
cmake ..
make
```

# Using
```
./httpfs --url="httpfs://google.com" munt_point
```
