pub use crate::ffi::{Keyword, PreLevel, PreTile, Tile};

static BUILTIN_LEVELS: phf::Map<&'static str, &'static str> = phf::phf_map! {
    "1" => include_str!("../resources/levels/1.json"),
    "2" => include_str!("../resources/levels/2.json"),
    "3" => include_str!("../resources/levels/3.json"),
    "4" => include_str!("../resources/levels/4.json"),
    "5" => include_str!("../resources/levels/5.json"),
    "6" => include_str!("../resources/levels/6.json"),
    "7" => include_str!("../resources/levels/7.json"),
    "8" => include_str!("../resources/levels/8.json"),
    "9" => include_str!("../resources/levels/9.json"),
};

impl PreLevel {
    pub fn from_file(file: &std::fs::File) -> anyhow::Result<Self> {
        serde_json::from_reader(file).map_err(|_e| anyhow::anyhow!("Failed to load level"))
    }

    pub fn from_builtin(level_name: &str) -> anyhow::Result<Self> {
        serde_json::from_str(
            BUILTIN_LEVELS
                .get(level_name)
                .ok_or(anyhow::anyhow!("Failed to get raw level"))?,
        )
        .map_err(|_e| anyhow::anyhow!("Failed to load level"))
    }
}

pub fn get_builtin_levels() -> Vec<String> {
    BUILTIN_LEVELS.keys().map(|s| s.to_string()).collect()
}
