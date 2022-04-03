// odgi-api.h ODGI C-API for FFIs
//
// Copyright (c) 2022 Erik Garrison and Pjotr Prins
//
// ODGI is published under the MIT license

#pragma once

#include <cstddef>

#include "odgi.hpp"

using namespace odgi;

// Some type conversions to serve faster passing of parameters and some
// type checking in the FFI. For basic types see
//   deps/libhandlegraph/src/include/handlegraph/util.hpp

// Introduce opaque types to support type checking of pointers to C++ classes
typedef struct opaque_graph {} *ograph_t;

// Use integers for the FFI
typedef uint64_t handle_i;
typedef uint64_t path_handle_i;
typedef unsigned __int128 edge_handle_i;
// step_handle_t is of type { char data[2 * sizeof(int64_t)]; };
// which we handle as int128 in the FFI, but for now:
typedef std::pair< int64_t, int64_t > pair64;

typedef pair64 step_handle_i;

inline handle_i as_handle_i(handle_t h) { return as_integer(h); };
inline handle_t as_handle_t(handle_i h) { return as_handle(h); };
inline path_handle_i as_path_handle_i(path_handle_t path) { return as_integer(path); };
inline path_handle_t as_path_handle_t(path_handle_i path) { return as_path_handle(path); };

inline const pair64 as_pair64(const step_handle_t step) {
  pair64 p = pair(step.data[0],step.data[1]);
  return p;
}

inline const step_handle_t as_step_handle(const pair64 p) {
  step_handle_t step;
  step.data[0] = p.first;   // handle
  step.data[1] = p.second;  // path
  return step;
}

inline step_handle_i as_step_handle_i(step_handle_t step) { return as_pair64(step); };
inline step_handle_t as_step_handle_t(step_handle_i step) { return as_step_handle(step); }


const std::string odgi_version();
size_t odgi_long_long_size();
size_t odgi_handle_i_size();
size_t odgi_edge_handle_i_size();
size_t odgi_step_handle_i_size();
unsigned __int128 odgi_test_uint128();
const ograph_t odgi_load_graph(const char *filen);
void odgi_free_graph(const ograph_t graph);
const size_t odgi_get_node_count(ograph_t graph);
const size_t odgi_max_node_id(const ograph_t graph);
const size_t odgi_min_node_id(const ograph_t graph);
const size_t odgi_get_path_count(ograph_t graph);
void odgi_for_each_path_handle(const ograph_t graph,
                               const std::function<void(const path_handle_i)>& next);
const bool odgi_for_each_handle(const ograph_t graph,
                                const std::function<bool(const handle_i)>& next);

const bool odgi_follow_edges(const ograph_t graph,
                             const handle_i ihandle,
                             bool go_left,
                             const std::function<bool(const handle_i ihandle)>& next);
const handle_i odgi_edge_first_handle(const ograph_t graph, const edge_t edge_handle);
const handle_i odgi_edge_second_handle(const ograph_t graph, const edge_t edge_handle);
const bool odgi_has_node(const ograph_t graph, nid_t node_id);
const std::string odgi_get_sequence(const ograph_t graph, const handle_i ihandle);
const nid_t odgi_get_id(const ograph_t graph, const handle_i ihandle);
const bool odgi_get_is_reverse(const ograph_t graph, const handle_i ihandle);
const size_t odgi_get_length(const ograph_t graph, const handle_i ihandle);
// Path handling
// const char *odgi_get_path_name(const ograph_t graph, const path_handle_i path);
const bool odgi_has_path(const ograph_t graph, const char *path_name);
const bool odgi_path_is_empty(const ograph_t graph, const path_handle_i path);
const path_handle_i odgi_get_path_handle(const ograph_t graph, const char *path_name);
// Steps
const size_t odgi_get_step_count(const ograph_t graph, const handle_i ihandle);
const handle_i odgi_get_handle_of_step(const ograph_t graph, step_handle_i step);
const path_handle_i odgi_get_path(const ograph_t graph, step_handle_i step);
const step_handle_i odgi_path_begin(const ograph_t graph, path_handle_i path);
const step_handle_i odgi_path_end(const ograph_t graph, path_handle_i path);
const step_handle_i odgi_path_back(const ograph_t graph, path_handle_i path);
const int64_t odgi_step_path_id(const ograph_t graph, step_handle_i step_handle);
bool odgi_step_is_reverse(const ograph_t graph, step_handle_i step_handle);
const int64_t odgi_step_prev_id(const ograph_t graph, step_handle_i step_handle);
const int64_t odgi_step_prev_rank(const ograph_t graph, step_handle_i step_handle);
const int64_t odgi_step_next_id(const ograph_t graph, step_handle_i step_handle);
const int64_t odgi_step_next_rank(const ograph_t graph,step_handle_i step_handle);
const bool odgi_step_eq(const ograph_t graph,
                        step_handle_i step_handle1,
                        step_handle_i step_handle2);
const step_handle_i odgi_path_front_end(const ograph_t graph,
                                   const path_handle_i path);
const step_handle_i odgi_get_next_step(const ograph_t graph,
                                       const step_handle_i step);
const step_handle_i odgi_get_previous_step(const ograph_t graph,
                                       const step_handle_i step);
const bool odgi_has_edge(const ograph_t graph, const handle_i left, const handle_i right);
const bool odgi_is_path_front_end(const ograph_t graph, const step_handle_i step);
const bool odgi_is_path_end(const ograph_t graph, const step_handle_i step);
const bool odgi_has_next_step(const ograph_t graph, const step_handle_i step);
const bool odgi_has_previous_step(const ograph_t graph, const step_handle_i step);
const path_handle_i odgi_get_path_handle_of_step(const ograph_t graph,
                                                 const step_handle_i step);
void odgi_for_each_step_in_path(const ograph_t graph,
                                const path_handle_i path,
                                void (*next) (const step_handle_i step));
const bool odgi_for_each_step_on_handle(const ograph_t graph,
                                        const handle_i ihandle,
                                        bool (*next) (const step_handle_i step));

const std::string odgi_get_path_name(const ograph_t graph, const path_handle_i ipath);

// Language agnostic C interface starts here

extern "C" {
  // const char *odgi_c_get_path_name(const ograph_t graph, const path_handle_i path);
}
