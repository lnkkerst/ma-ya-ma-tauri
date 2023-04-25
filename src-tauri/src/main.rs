// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("include/mayama.h");
        fn add(x: i32, y: i32) -> i32;
    }
}

// Learn more about Tauri commands at https://tauri.app/v1/guides/features/command
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[tauri::command]
fn add(x: i32, y: i32) -> i32 {
    ffi::add(x, y)
}

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![greet, add])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
