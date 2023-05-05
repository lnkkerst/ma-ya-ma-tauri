pub use crate::ffi::Theme;

static BUILTIN_THEMES: phf::Map<&'static str, &'static str> = phf::phf_map! {
    "default" => include_str!("../resources/themes/default.json"),
};

impl Theme {
    pub fn from_builtin(theme_name: &str) -> anyhow::Result<Self> {
        serde_json::from_str(
            BUILTIN_THEMES
                .get(&theme_name)
                .ok_or(anyhow::anyhow!("Failed to load raw theme"))?,
        )
        .map_err(|_e| anyhow::anyhow!("Failed to load theme"))
    }
}
