import { invoke } from '@tauri-apps/api';
import { defineComponent, onMounted, ref } from 'vue';
import { useRouter } from 'vue-router';
import BackToHome from '~/components/BackToHome';
import Tile from '~/components/Tile';

export default defineComponent({
  setup() {
    const router = useRouter();
    const levels = ref<string[]>([]);

    onMounted(async () => {
      levels.value = (
        (await invoke('get_builtin_level_list')) as string[]
      ).sort((a, b) => parseInt(a) - parseInt(b));
    });

    return () => (
      <div mt="1/10">
        <div font="bold" text="3vh" mx-auto text-center>
          选择关卡
        </div>
        <div grid grid-cols-4>
          {levels.value.map(level => (
            <div
              p="1/10"
              onClick={() => router.push({ path: '/game', query: { level } })}
            >
              <Tile text={level} font-size="200%"></Tile>
            </div>
          ))}
        </div>
        <BackToHome class={['absolute', 'right-1/25', 'top-1/50']}></BackToHome>
      </div>
    );
  }
});
