import { invoke } from '@tauri-apps/api';
import { defineComponent, onMounted, ref } from 'vue';
import { useRoute, useRouter } from 'vue-router';
import BackToHome from '~/components/BackToHome';
import Tile from '~/components/Tile';
import useWindowScale from '~/composables/windowScale';

export default defineComponent({
  setup() {
    const route = useRoute();
    const router = useRouter();
    const scale = useWindowScale();
    const levels = ref<string[]>([]);
    const redirect = (route.query.redirect as string) ?? '/records';

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
        <div
          grid
          grid-cols-4
          style={{
            margin: `${scale.value * 16}px ${scale.value * 32}px`,
            height: `${scale.value * 960}px`
          }}
          overflow-y-auto
        >
          {levels.value.map(level => (
            <div
              style={{ padding: `${scale.value * 16}px` }}
              onClick={() => router.push({ path: redirect, query: { level } })}
            >
              <Tile cursor-pointer text={level} font-size="200%"></Tile>
            </div>
          ))}
        </div>
        <BackToHome></BackToHome>
      </div>
    );
  }
});
