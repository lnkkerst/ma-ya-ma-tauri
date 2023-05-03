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
  board: [],
  tiles: [
    {
      id: '12312313342',
      row: 1,
      column: 1,
      index: 0,
      exposed: true,
      keyword: {
        content: 'let',
        color: variants.latte.mauve.hex
      },
      onBuffer: false,
      dropped: false,
      selected: false
    },
    {
      id: '1231231334dede',
      row: 1,
      column: 1,
      index: 1,
      exposed: true,
      keyword: {
        content: 'mut',
        color: variants.latte.mauve.hex
      },
      onBuffer: false,
      dropped: false,
      selected: false
    },
    {
      id: '1231231334dadanb',
      row: 1,
      column: 1,
      index: 1,
      exposed: true,
      keyword: {
        content: 'mut',
        color: variants.latte.mauve.hex
      },
      onBuffer: false,
      dropped: false,
      selected: false
    }
  ],
  buffer: []
});

export default function useGameState() {
  return gameState;
}
