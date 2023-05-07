import { defineComponent } from 'vue';
import { useRouter } from 'vue-router';
import { VBtn } from 'vuetify/components';
import { appWindow } from '@tauri-apps/api/window';

export default defineComponent({
  setup() {
    const router = useRouter();

    return () => (
      <div>
        <div py="1/5" text="15" text-center cursor-default>
          码呀码
        </div>
        <div flex flex-col items-center justify-center w-full gap-12>
          <VBtn
            variant="outlined"
            size="large"
            width="128"
            onClick={() =>
              router.push({ path: '/levels', query: { redirect: '/game' } })
            }
          >
            开始游戏
          </VBtn>
          <VBtn
            variant="outlined"
            size="large"
            width="128"
            onClick={() =>
              router.push({ path: '/levels', query: { redirect: '/records' } })
            }
          >
            排行榜
          </VBtn>
          <VBtn
            variant="outlined"
            size="large"
            width="128"
            onClick={() => appWindow.close()}
          >
            退出游戏
          </VBtn>
        </div>
      </div>
    );
  }
});
