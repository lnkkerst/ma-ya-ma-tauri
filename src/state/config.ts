import { useLocalStorage } from '@vueuse/core';
import { storagePrefix } from '~/consts';

export interface AppConfig {
  tileFont: 'jetbrains-mono' | 'fira-code';
  canvasNest: boolean;
}

export function useAppConfig() {
  return useLocalStorage<AppConfig>(
    `${storagePrefix}-config`,
    { tileFont: 'jetbrains-mono', canvasNest: true },
    { mergeDefaults: true }
  );
}
