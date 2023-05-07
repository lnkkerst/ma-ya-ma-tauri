import { useWindowSize } from '@vueuse/core';
import { computed } from 'vue';
import { appSize } from '~/consts';

export default function useWindowScale() {
  const { width: windowWidth, height: windowHeight } = useWindowSize();
  const scale = computed(() =>
    Math.min(
      windowWidth.value / appSize.width,
      windowHeight.value / appSize.height
    )
  );
  return scale;
}
