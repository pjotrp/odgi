fn main() {
    cxx_build::bridge("rust/main.rs")
        .file("rust/blobstore.cc")
        .flag_if_supported("-std=c++17")
        .compile("cxxbridge-demo");

    println!("cargo:rerun-if-changed=rust/main.rs");
    println!("cargo:rerun-if-changed=rust/blobstore.cc");
    println!("cargo:rerun-if-changed=rust/blobstore.h");
}
