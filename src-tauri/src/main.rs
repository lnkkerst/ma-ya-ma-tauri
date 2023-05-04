// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use ma_ya_ma::ffi;

// Learn more about Tauri commands at https://tauri.app/v1/guides/features/command
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[tauri::command]
fn test() -> ffi::Keyword {
    ffi::Keyword {
        content: "let".into(),
        keyword_type: "normal".into(),
        color: "red".into(),
        background_color: "black".into(),
    }
}

#[tauri::command]
fn handle_click_tile(tile: Vec<ffi::Tile>) {
    println!("{:?}", tile);
}

#[tauri::command]
fn init_game(tiles: Vec<ffi::Tile>) {
    println!("{:?}", tiles);
}

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![
            greet,
            test,
            handle_click_tile,
            init_game
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
