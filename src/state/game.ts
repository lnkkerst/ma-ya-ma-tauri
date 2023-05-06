import { ref } from 'vue';
import { GameStatus } from '~/types';
import type { Tile } from '~/types';

export interface GameState {
  tiles: Tile[];
  board: Tile[];
  buffer: Tile[];
  score: number;
  status: GameStatus;
  tilesMap: Record<string, Tile>;
  movingTile: string;
}

const gameState = ref<GameState>({
  status: GameStatus.Running,
  score: 0,
  board: [],
  tiles: [],
  buffer: [],
  tilesMap: {},
  movingTile: ''
});

export default function useGameState() {
  return gameState;
}
