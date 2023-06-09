// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use ma_ya_ma::*;

// Learn more about Tauri commands at https://tauri.app/v1/guides/features/command
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[tauri::command]
fn handle_click_tile(tile: ffi::Tile) -> ffi::ClickTileResult {
    ffi::handle_click_tile(&tile).as_ref().unwrap().clone()
}

#[tauri::command]
fn load_level_from_builtin(level_name: String) -> Result<Vec<ffi::Tile>, String> {
    let pre_level =
        level::PreLevel::from_builtin(&level_name).map_err(|_e| "Failed to load level")?;
    ffi::load_tiles(&pre_level);
    ffi::init_game();
    Ok(ffi::get_tiles().value)
}

#[tauri::command]
fn load_level_from_custom(pre_level: level::PreLevel) -> Result<Vec<ffi::Tile>, String> {
    ffi::load_tiles(&pre_level);
    ffi::init_game();
    Ok(ffi::get_tiles().value)
}

#[tauri::command]
fn load_theme_from_builtin(theme_name: String) -> Result<(), String> {
    let theme = theme::Theme::from_builtin(&theme_name).map_err(|_e| "Failed to load theme")?;
    ffi::load_theme(&theme);
    Ok(())
}

#[tauri::command]
fn get_status() -> ffi::GameStatus {
    ffi::get_status()
}

#[tauri::command]
fn get_score() -> i32 {
    ffi::get_score()
}

#[tauri::command]
fn get_builtin_level_list() -> Vec<String> {
    level::get_builtin_levels()
}

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![
            greet,
            handle_click_tile,
            load_level_from_builtin,
            load_theme_from_builtin,
            get_status,
            get_score,
            get_builtin_level_list,
            load_level_from_custom
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
