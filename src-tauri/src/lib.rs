pub mod level;
pub mod theme;

#[cxx::bridge]
pub mod ffi {
    #[derive(Debug, Deserialize, Serialize, Clone)]
    struct Keyword {
        content: String,
        #[serde(rename = "type")]
        #[cxx_name = "type"]
        keyword_type: String,
        color: String,
        #[serde(rename = "backgroundColor")]
        background_color: String,
    }

    #[derive(Debug, Deserialize, Serialize, Clone)]
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

    #[derive(Debug, Serialize, Deserialize, Clone)]
    struct Tiles {
        value: Vec<Tile>,
    }

    #[derive(Debug, Serialize, Deserialize, Clone)]
    pub struct PreTile {
        column: i32,
        row: i32,
        index: i32,
    }

    #[derive(Debug, Deserialize, Serialize, Clone)]
    pub struct PreLevel {
        id: String,
        name: String,
        tiles: Vec<PreTile>,
    }

    #[derive(Debug, Deserialize, Serialize, Clone)]
    struct KeywordGroup {
        contents: Vec<String>,
    }

    #[derive(Debug, Deserialize, Serialize, Clone)]
    pub struct Theme {
        id: String,
        name: String,
        keywords: Vec<Keyword>,
        groups: Vec<KeywordGroup>,
    }

    #[derive(Debug, Deserialize, Serialize, Clone)]
    pub struct TileDiff {
        id: String,
        diff: Vec<String>,
    }

    #[derive(Debug, Deserialize, Serialize, Clone)]
    pub struct ClickTileResult {
        diffs: Vec<TileDiff>,
    }

    #[derive(Debug, Deserialize, Serialize, Clone)]
    enum GameEvent {
        ClickTile,
    }

    #[derive(Debug, Deserialize, Serialize, Clone)]
    enum GameStatus {
        Running,
        Winned,
        Losed,
    }

    unsafe extern "C++" {
        include!("include/mayama.hpp");
        include!("include/Game.hpp");

        type Game;
        fn get_status() -> GameStatus;
        fn load_tiles(level: &PreLevel) -> ();
        fn load_theme(theme: &Theme) -> ();
        fn get_tiles() -> Tiles;
        fn init_game() -> ();
        fn handle_click_tile(tile: &Tile) -> UniquePtr<ClickTileResult>;
        fn get_score() -> i32;
    }
}
