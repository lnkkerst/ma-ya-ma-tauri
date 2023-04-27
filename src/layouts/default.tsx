import { useWindowSize } from '@vueuse/core';
import { computed, defineComponent, onMounted, ref, watch } from 'vue';
import { RouterView } from 'vue-router';

// @ts-expect-error no declaration file
import CanvasNest from 'canvas-nest.js';
import { variants } from '@catppuccin/palette';
import { appSize } from '~/consts';
import { useAppConfig } from '~/state/config';

export default defineComponent({
  setup() {
    const appConfig = useAppConfig();
    const { width: windowWidth, height: windowHeight } = useWindowSize();

    const el = ref<HTMLDivElement | null>(null);
    const scale = computed(() =>
      Math.min(
        windowWidth.value / appSize.width,
        windowHeight.value / appSize.height
      )
    );
    let cn: CanvasNest | null = null;

    onMounted(() => {
      watch(
        () => appConfig.value.canvasNest,
        val => {
          if (import.meta.env.DEV) {
            return;
          }
          if (!el.value) {
            return;
          }
          if (val && !cn) {
            cn = new CanvasNest(el.value, {
              color: variants.latte.text,
              zIndex: 0,
              count: 66
            });
          }
          if (!val && cn) {
            cn.destory();
          }
        },
        { immediate: true }
      );
    });

    return () => (
      <div relative grid place-items="center" w="screen" h="screen" bg="black">
        <div
          ref={el}
          bg-latte-base
          z="0"
          absolute
          style={{
            height: `${appSize.height * scale.value}px`,
            width: `${appSize.width * scale.value}px`,
            aspectRatio: '9 / 16'
          }}
        >
          <RouterView></RouterView>
        </div>
      </div>
    );
  }
});
