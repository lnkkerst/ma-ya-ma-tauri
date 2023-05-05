import { invoke } from '@tauri-apps/api';
import { defineComponent } from 'vue';
import { useRoute } from 'vue-router';
import Board from '~/components/Board';
import useGameState from '~/state/game';

export default defineComponent({
  setup() {
    const route = useRoute();
    const levelName = route.params.level || '1';
    const gameState = useGameState();
    invoke('load_theme_from_builtin', { themeName: 'default' });
    invoke('load_level_from_builtin', { levelName });
    return () => (
      <>
        <Board></Board>
      </>
    );
  }
});
