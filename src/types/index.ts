export interface Keyword {
  content: string;
  type: 'normal' | 'wildcard';
  color: string;
  backgroundColor: string;
}

export interface Tile {
  id: string;
  row: number;
  column: number;
  index: number;
  exposed: boolean;
  keyword: Keyword;
  onBuffer: boolean;
  dropped: boolean;
  selected: boolean;
}

export interface ClickTileResult {
  diffs: { id: string; diff: string[] }[];
}

export enum GameStatus {
  Running,
  Winned,
  Losed
}

export interface GameRecord {
  name: string;
  score: number;
}
