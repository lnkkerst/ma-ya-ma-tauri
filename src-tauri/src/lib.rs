mod tile;

#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("include/mayama.h");
        fn add(x: i32, y: i32) -> i32;
    }
}
