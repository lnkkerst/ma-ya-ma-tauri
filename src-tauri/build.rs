#[allow(unused_must_use)]
fn main() {
    cxx_build::CFG.include_prefix = "";
    cxx_build::bridge("src/lib.rs");
    tauri_build::build();

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rustc-link-lib=static=ma_ya_ma");
    println!(r"cargo:rustc-link-search=native=../build/");
}
