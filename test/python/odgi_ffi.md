% -*- coding: utf-8 -*-

# ODGI Python FFI

The odgi toolkit for pangenomics comes with a simple "C" foreign function interface (FFI) that can be used from any computer language.
The header file for the C-API can be found [here](https://github.com/pjotrp/odgi/blob/master/src/odgi-api.h).
In this document we walk through the low-level API using the Python `odgi_ffi` module that comes with odgi.

Note that odgi also has an older high-level Python API `import odgi` that is somewhat obsolete. Instead you should probably use below `import odgi_ffi` lower level API to construct your own library.

The main purpose of this FFI is to allow other languages to explore an in-memory odgi pangenome.

## Setting it up

First import the module. It may require setting the `PYTHONPATH` to the shared library `odgi_ffi.cpython-39-x86_64-linux-gnu.so`. Also it may be necessary to preload jemalloc with:

    env LD_PRELOAD=libjemalloc.so.2 PYTHONPATH=/odgi/lib python3 -c 'import odgi_ffi'

in a GNU Guix shell you may prepend `LD_LIBRARY_PATH` to find GLIBC etc.

    LD_LIBRARY_PATH=$LIBRARY_PATH

Now you should be able to use the `odgi_ffi` module and load the graph with 3214 nodes and 12 paths

```python
>>> from odgi_ffi import *

>>> graph = odgi_load_graph("DRB1-3123_sorted.og")
>>> odgi_get_node_count(graph)
3214

>>> odgi_get_path_count(graph)
12

```

Note that load graph reads the `.og` file format only. To read a `GFA` file convert that using the odgi tool. E.g..

```sh
    odgi build -g DRB1-3123.gfa -o DRB1-3123.og
```

## API conventions

The function names for odgi_ffi reflect the namings in the ODGI C++ library. This to avoid confusion when digging into internals.
To facilitate different language FFIs we allow for modern C++ constructs (`odgi_` prefix) or use simple C bindings (`odgi_c_` prefix). The modern C++ constructs, including unique_ptr, can be very attractive for type checking and elegant garbage collecting.
Next, by default, data is copied across language barriers. For most odgi handles this is no problem as they are long integers. But if it is text it may be useful not to copy data to speed up processing (or to modify data in place). In this case we use the `odgi_raw_` and `odgi_c_raw_` prefixes. It is ugly, but when defining the API in your favorite language there is chance of renaming or aliasing function names to get a more canonical representation. For Python, following these conventions, to get the path name we can potentially use:

```python
imort odgi_ffi

# These are examples of naming convention:
odgi_get_path_name(graph,p)       # C++ copy handler (default)
odgi_raw_get_path_name(graph,p)   # C++ string access in place
odgi_c_get_path_name(graph,p)     # C copy handler (default)
odgi_c_raw_get_path_name(graph,p) # C string access in place
```

Note that not all these functions may be implemented. See the reference (FIXME).

Since graph and p represent state we can move it into a class and end up with a nice canonical high-level interface `import odgi_graph` which we will use in the nice API document (FIXME).

In this document we only discuss the native low-level API.

## Exploring the pagenome

A pangenome is made out of nodes (sequences), edges (connectors) and paths:

![Path through pangenome](../../docs/img/exampleGraphPath.png "Pangenome path")

A path represents one version of a genome - it can represent an individual, or a version of a gene. Anything that is a linked sequence. In the picture the red path represents `CGA TTGG CCGT GT GATAA CGG ACA ATATAAC'.

In the DRB1-3123 pangenome graph that we loaded with odgi we have 12 paths.
Show the names using a call back

```python

>>> def test(p):
...   print(odgi_get_path_name(graph,p))
...   print(odgi_c_get_path_name(graph,p))
...   return True

>>> list = []
>>> odgi_for_each_path_handle2(graph, lambda p: test(p))
gi|568815592:32578768-32589835
gi|568815529:3998044-4011446
gi|568815551:3814534-3830133
gi|568815561:3988942-4004531
gi|568815567:3779003-3792415
gi|568815569:3979127-3993865
gi|345525392:5000-18402
gi|29124352:124254-137656
gi|28212469:126036-137103
gi|28212470:131613-146345
gi|528476637:32549024-32560088
gi|157702218:147985-163915

odgi_free_graph(graph)

```


path_name5 = nil
ODGI.each_path(pangenome) { |path|
  p [path,ODGI.path_name(pangenome,path)]
  path_name5 = ODGI.path_name(pangenome,path) if path==5
}

raise "No path[5]" if not ODGI.has_path(pangenome,path_name5)

path5 = ODGI::get_path(pangenome,path_name5);
p [path5,ODGI.path_name(pangenome,path5)]

ODGI.each_handle(pangenome) { |handle|
  right_edges = []
  ODGI::follow_edges(pangenome,handle,true) { |edge|
    right_edges.push edge
  }
  left_edges = []
  ODGI::follow_edges(pangenome,handle,false) { |edge|
    left_edges.push edge
  }
  p [handle,ODGI::get_id(pangenome,handle),ODGI::get_sequence(pangenome,handle),left_edges,right_edges]
}