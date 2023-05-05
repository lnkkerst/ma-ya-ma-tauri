use std::{env, path::Path};

use glob::glob;

#[allow(unused_must_use)]
fn main() {
    cxx_build::CFG.include_prefix = "";
    let mut cxx_bridge = cxx_build::bridge("src/lib.rs");
    if let Ok(v) = env::var("CMAKE_TARGET_LIBRARY") {
        println!("cargo:rustc-link-lib=static=ma_ya_ma");
        println!(
            "cargo:rustc-link-search=native={}",
            Path::new(&v).parent().unwrap().to_str().unwrap()
        );
    } else {
        let files = glob("../src-cpp/*.cc")
            .unwrap()
            .map(|p| p.unwrap().to_str().unwrap().to_string())
            .collect::<Vec<_>>();

        cxx_bridge
            .files(&files)
            .flag_if_supported("-std=c++14")
            .compile("ma_ya_ma");
        files
            .iter()
            .for_each(|f| println!("cargo:rerun-if-changed={}", f));
    }
    tauri_build::build();

    println!("cargo:rerun-if-changed=src/lib.rs");
}
