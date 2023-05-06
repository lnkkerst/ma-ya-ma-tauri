import { invoke } from '@tauri-apps/api';
import { defineAsyncComponent, ref } from 'vue';
import { useRoute } from 'vue-router';
import { gsap } from 'gsap';
import Board from '~/components/Board';
import useGameState from '~/state/game';
import type { Tile } from '~/types';

export default defineAsyncComponent({
  suspensible: false,
  async loader() {
    const route = useRoute();
    const levelName = route.params.level || '1';
    const gameState = useGameState();
    const scoreEl = ref<HTMLDivElement | null>(null);
    await invoke('load_theme_from_builtin', { themeName: 'default' });
    gameState.value.tiles = (await invoke('load_level_from_builtin', {
      levelName
    })) as Tile[];
    gameState.value.tiles.forEach(tile => {
      gameState.value.tilesMap[tile.id] = tile;
    });

    async function handleClickTile() {
      setTimeout(updateScore, 350);
      const _status = (await invoke('get_status')) as { repr: number };
    }

    async function updateScore() {
      const newScore = (await invoke('get_score')) as number;
      if (gameState.value.score !== newScore) {
        gameState.value.score = newScore;
        if (!scoreEl.value) {
          return;
        }
        gsap.to(scoreEl.value, {
          scale: 1.1,
          repeat: 1,
          duration: 0.2,
          yoyo: true
        });
      }
    }

    return () => (
      <div>
        <div
          absolute
          top="2/100"
          left="4/100"
          ref={scoreEl}
          font="bold"
          style={{ fontSize: '2vh' }}
        >{`得分：${gameState.value.score}`}</div>
        <Board onClickTile={handleClickTile}></Board>
      </div>
    );
  }
});
