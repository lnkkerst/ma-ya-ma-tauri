export interface Keyword {
  content: string;
  color?: string;
  backgroundColor?: string;
}

export interface Tile {
  row: number;
  column: number;
  index: number;
  exposed: boolean;
  keyword: Keyword;
}
