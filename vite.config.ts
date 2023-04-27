import path from 'node:path';
import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';
import vueJsx from '@vitejs/plugin-vue-jsx';
import Icons from 'unplugin-icons/vite';
import Pages from 'vite-plugin-pages';
import Layouts from 'vite-plugin-vue-layouts';
import unocss from 'unocss/vite';
import {
  presetAttributify,
  presetIcons,
  presetTagify,
  presetTypography,
  presetUno,
  presetWebFonts,
  transformerDirectives
} from 'unocss';
import transformerAttributifyJsx from '@unocss/transformer-attributify-jsx-babel';
import type { Theme } from 'unocss/preset-uno';
import { extendCatppuccin } from 'unocss-catppuccin-colours';

// https://vitejs.dev/config/
export default defineConfig(async () => ({
  plugins: [
    vue(),
    vueJsx(),
    unocss<Theme>({
      presets: [
        presetUno(),
        presetIcons(),
        presetWebFonts({
          provider: 'google',
          fonts: {
            'jetbrains-mono': ['JetBrains Mono'],
            'fira-code': ['FiraCode']
          }
        }),
        presetTypography(),
        presetAttributify(),
        presetTagify()
      ],
      transformers: [transformerAttributifyJsx(), transformerDirectives()],
      theme: {
        colors: extendCatppuccin()
      }
    }),
    Pages({
      extensions: ['vue', 'tsx', 'jsx']
    }),
    Layouts({
      extensions: ['ts', 'js', 'tsx', 'jsx']
    }),
    Icons({})
  ],

  // Vite options tailored for Tauri development and only applied in `tauri dev` or `tauri build`
  // prevent vite from obscuring rust errors
  clearScreen: false,
  // tauri expects a fixed port, fail if that port is not available
  server: {
    port: 1420,
    strictPort: true
  },
  // to make use of `TAURI_DEBUG` and other env variables
  // https://tauri.studio/v1/api/config#buildconfig.beforedevcommand
  envPrefix: ['VITE_', 'TAURI_'],
  build: {
    // Tauri supports es2021
    target: process.env.TAURI_PLATFORM === 'windows' ? 'chrome105' : 'safari13',
    // don't minify for debug builds
    minify: !process.env.TAURI_DEBUG ? 'esbuild' : false,
    // produce sourcemaps for debug builds
    sourcemap: !!process.env.TAURI_DEBUG
  },
  resolve: {
    alias: {
      '~/': `${path.resolve(__dirname, 'src')}/`
    }
  }
}));
