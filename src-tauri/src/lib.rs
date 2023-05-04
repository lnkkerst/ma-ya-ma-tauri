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

        fn new_game() -> UniquePtr<Game>;
        fn get_status(self: &Game) -> GameStatus;
    }
}
