# WWWFS

This project is aiming to create a http(s) based file system, that parses HTML and serves it as a filesystem. 
`<a>` links pointing to other HTML pages are represented as folders, links pointed to other resources are represented as files. Other tags, such as `<img>`, `<video>`, `<audio>` are also parsed and represented as files.

There are other projects that are trying to achieve something simmilar, but these are not quite as awesome:
* http://httpfs.sourceforge.net/ - allows to mount only one file over http
* https://github.com/cyrus-and/httpfs - requires server side php script

# Current state

Implemented HTML parsing prototype using [QGumboParser](https://github.com/lagner/QGumboParser). Http client rewritten with Promis-like callbacks. Implemented cache. Everything works 95% of time. There is still probably a bug in caching or http client.

~~Found a problem: some servers do not return content-length, so http client must be rewritten...~~

~~Currently project works only with my own localhost server, because HTML parser is not yet implemented and I had to hardcode parsed elements manually.~~

Cache is mocked too, no real implementation yet. Most likely I will have to rewrite it to handle identical parallel requests better.

~~Currently project serves only two hardcoded files. As of now http(s) client side and parser is missing. Http client will be finished on next free weekend.~~

Proof of concept was rewritten to support async lowlevel api and now depends only on fuse and qt.

~~Currently project is in "proof of concept" state. For now it only allows reading files in a very inefficient way and there is no folder support.~~

TODO:
* Write tests for cache
* Support more HTML tags
* Symlink support
* create virtual file _index.html for debugging reasons
* DBus progress indicator???

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
