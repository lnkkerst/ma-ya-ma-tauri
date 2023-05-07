import { invoke } from '@tauri-apps/api';
import { defineComponent, onMounted, ref } from 'vue';
import { useRoute, useRouter } from 'vue-router';
import { open } from '@tauri-apps/api/dialog';
import { readTextFile } from '@tauri-apps/api/fs';
import BackToHome from '~/components/BackToHome';
import Tile from '~/components/Tile';
import type { Tile as TileType } from '~/types';
import useWindowScale from '~/composables/windowScale';
import useGameState from '~/state/game';

export default defineComponent({
  setup() {
    const route = useRoute();
    const router = useRouter();
    const scale = useWindowScale();
    const gameState = useGameState();
    const levels = ref<string[]>([]);
    const redirect = (route.query.redirect as string) ?? '/records';

    onMounted(async () => {
      levels.value = (
        (await invoke('get_builtin_level_list')) as string[]
      ).sort((a, b) => parseInt(a) - parseInt(b));
    });

    async function handleLoadCustomLevel() {
      const selected = await open({
        multiple: false,
        directory: false
      });
      if (!selected) {
        return;
      }
      try {
        const preLevel = JSON.parse(await readTextFile(selected as string));
        await invoke('load_theme_from_builtin', { themeName: 'default' });
        gameState.value.customLevel = {
          tiles: (await invoke('load_level_from_custom', {
            preLevel
          })) as TileType[]
        };
        router.push({ path: '/game', query: { level: 'custom' } });
      } catch (e) {
        alert('加载失败');
      }
    }

    return () => (
      <div mt="1/10">
        <div font="bold" text="3vh" mx-auto text-center>
          选择关卡
        </div>
        <div
          grid
          grid-cols-4
          style={{
            margin: `${scale.value * 128}px ${scale.value * 32}px`,
            height: `${scale.value * 540}px`
          }}
          overflow-y-auto
        >
          {levels.value.map(level => (
            <div
              style={{ padding: `${scale.value * 16}px`, aspectRatio: '1 / 1' }}
              onClick={() => router.push({ path: redirect, query: { level } })}
            >
              <Tile cursor-pointer text={level} font-size="200%"></Tile>
            </div>
          ))}
          {redirect.startsWith('/game') ? (
            <div
              style={{ padding: `${scale.value * 16}px`, aspectRatio: '1 / 1' }}
              onClick={handleLoadCustomLevel}
            >
              <Tile cursor-pointer text="自定义" font-size="100%"></Tile>
            </div>
          ) : undefined}
        </div>
        <BackToHome></BackToHome>
      </div>
    );
  }
});
