import {
  Suspense,
  Transition,
  defineComponent,
  onMounted,
  onUnmounted,
  ref,
  watch
} from 'vue';
import { RouterView } from 'vue-router';

// @ts-expect-error no declaration file
import CanvasNest from 'canvas-nest.js';
import { variants } from '@catppuccin/palette';
import { appSize } from '~/consts';
import { useAppConfig } from '~/state/config';
import useWindowScale from '~/composables/windowScale';

export default defineComponent({
  setup() {
    const appConfig = useAppConfig();

    const el = ref<HTMLDivElement | null>(null);
    const scale = useWindowScale();
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

    onUnmounted(() => {
      if (cn) {
        cn.destory();
      }
    });

    return () => (
      <Suspense>
        {{
          default: () => (
            <div
              relative
              flex
              justify-center
              items-center
              w="screen"
              h="screen"
              bg="black"
            >
              <div
                ref={el}
                bg-latte-base
                z="0"
                absolute
                overflow-hidden
                style={{
                  height: `${appSize.height * scale.value}px`,
                  width: `${appSize.width * scale.value}px`,
                  aspectRatio: '9 / 16'
                }}
                select-none
              >
                {/* <RouterView> */}
                {/*   {{ */}
                {/*     default: ({ Component }) => ( */}
                {/*       <Transition name="page">{Component}</Transition> */}
                {/*     ) */}
                {/*   }} */}
                {/* </RouterView> */}
                <Transition name="page">
                  <RouterView></RouterView>
                </Transition>
              </div>
            </div>
          ),
          fallback: () => <div>loading...</div>
        }}
      </Suspense>
    );
  }
});
