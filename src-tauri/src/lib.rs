pub mod level;
pub mod theme;

#[cxx::bridge]
pub mod ffi {
    #[derive(Debug, Deserialize, Serialize)]
    struct Keyword {
        content: String,
        #[serde(rename = "type")]
        #[cxx_name = "type"]
        keyword_type: String,
        color: String,
        #[serde(rename = "backgroundColor")]
        background_color: String,
    }

    #[derive(Debug, Deserialize, Serialize)]
    struct Tile {
        id: String,
        row: i32,
        column: i32,
        index: i32,
        exposed: bool,
        keyword: Keyword,
        #[serde(rename = "onBuffer")]
        on_buffer: bool,
        dropped: bool,
        selected: bool,
    }

    #[derive(Debug)]
    struct Tiles {
        value: Vec<Tile>,
    }

    #[derive(Debug, Serialize, Deserialize)]
    pub struct PreTile {
        column: i32,
        row: i32,
        index: i32,
    }

    #[derive(Debug, Deserialize, Serialize)]
    pub struct PreLevel {
        id: String,
        name: String,
        tiles: Vec<PreTile>,
    }

    #[derive(Debug, Deserialize, Serialize)]
    struct KeywordGroup {
        contents: Vec<String>,
    }

    #[derive(Debug, Deserialize, Serialize)]
    pub struct Theme {
        id: String,
        name: String,
        keywords: Vec<Keyword>,
        groups: Vec<KeywordGroup>,
    }

    enum GameEvent {
        ClickTile,
    }

    enum GameStatus {
        Running,
        Winned,
        Losed,
    }

    unsafe extern "C++" {
        include!("include/mayama.h");

        type Game;
        fn get_status() -> GameStatus;
        fn load_tiles(level: &PreLevel) -> ();
        fn load_theme(theme: &Theme) -> ();
        fn get_tiles() -> Tiles;
    }
}
