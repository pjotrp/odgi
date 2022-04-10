use cxx_build::CFG;
use std::fs;

fn main() {
    //use std::path::{Path};

    let handlegraph = fs::canonicalize("./deps/libhandlegraph/src/include").unwrap();
    println!("Include paths: {}",handlegraph.to_str().unwrap());
    CFG.exported_header_dirs.push(&handlegraph);

    let mmulti = fs::canonicalize("./deps/mmmulti/deps/DYNAMIC/include").unwrap();
    CFG.exported_header_dirs.push(&mmulti);
    let hopscotch = fs::canonicalize("./deps/hopscotch-map/include/").unwrap();
    CFG.exported_header_dirs.push(&hopscotch);

    cxx_build::bridge("rust/main.rs")
        .file("rust/blobstore.cc")
        .flag_if_supported("-std=c++17")
        .compile("cxxbridge-demo");

    println!("cargo:rerun-if-changed=rust/main.rs");
    println!("cargo:rerun-if-changed=rust/blobstore.cc");
    println!("cargo:rerun-if-changed=rust/blobstore.h");
    println!("cargo:rerun-if-changed=src/odgi-api.h");
}
