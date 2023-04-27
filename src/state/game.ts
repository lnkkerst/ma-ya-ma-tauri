import { variants } from '@catppuccin/palette';
import { ref } from 'vue';
import type { Tile } from '~/types';

export interface GameState {
  tiles: Tile[];
  board: Tile[];
  buffer: Tile[];
  score: number;
  status: 'preparing' | 'running' | 'win' | 'lose';
}

const gameState = ref<GameState>({
  status: 'preparing',
  score: 0,
  tiles: [],
  board: [
    {
      row: 1,
      column: 1,
      index: 0,
      exposed: true,
      keyword: {
        content: 'let',
        color: variants.latte.mauve.hex
      }
    },
    {
      row: 1,
      column: 1,
      index: 1,
      exposed: true,
      keyword: {
        content: 'mut',
        color: variants.latte.mauve.hex
      }
    }
  ],
  buffer: []
});

export default function useGameState() {
  return gameState;
}
