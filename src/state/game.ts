import { ref } from 'vue';
import type { Tile } from '~/types';

export interface GameState {
  tiles: Tile[];
  board: Tile[];
  buffer: Tile[];
  score: number;
  status: 'preparing' | 'running' | 'win' | 'lose';
  tilesMap: Record<string, Tile>;
  movingTile: string;
}

const gameState = ref<GameState>({
  status: 'preparing',
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
