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
