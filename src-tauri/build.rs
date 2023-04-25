fn main() {
    cxx_build::CFG.include_prefix = "";
    cxx_build::bridge("src/main.rs")
        .file("src/mayama.cc")
        .flag_if_supported("-std=c++14")
        .compile("ma-ya-ma");
    tauri_build::build();
    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=include/mayama.h");
    println!("cargo:rerun-if-changed=src/mayama.cc");
}
