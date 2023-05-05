import { invoke } from '@tauri-apps/api';
import { defineAsyncComponent } from 'vue';
import { useRoute } from 'vue-router';
import Board from '~/components/Board';
import useGameState from '~/state/game';
import type { Tile } from '~/types';

export default defineAsyncComponent({
  suspensible: false,
  async loader() {
    const route = useRoute();
    const levelName = route.params.level || '1';
    const gameState = useGameState();
    await invoke('load_theme_from_builtin', { themeName: 'default' });
    gameState.value.tiles = (await invoke('load_level_from_builtin', {
      levelName
    })) as Tile[];
    gameState.value.tiles.forEach(tile => {
      gameState.value.tilesMap[tile.id] = tile;
    });

    return () => (
      <>
        <Board></Board>
      </>
    );
  }
});
