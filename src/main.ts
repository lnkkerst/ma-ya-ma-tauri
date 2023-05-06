import { createApp } from 'vue';
import { createHead } from '@vueuse/head';
import { MotionPlugin } from '@vueuse/motion';
import { createRouter, createWebHashHistory } from 'vue-router';
import { createVuetify } from 'vuetify';
import { zhHans } from 'vuetify/locale';
import { aliases, mdi } from 'vuetify/iconsets/mdi';

import '@unocss/reset/tailwind.css';
import 'virtual:uno.css';
import 'vuetify/styles';
import '~/styles/main.scss';
import '@mdi/font/css/materialdesignicons.css';

import generatedRoutes from 'virtual:generated-pages';
import { setupLayouts } from 'virtual:generated-layouts';
import App from './App';

const routes = setupLayouts(generatedRoutes);
const head = createHead();
const router = createRouter({
  routes,
  history: createWebHashHistory()
});
const vuetify = createVuetify({
  locale: {
    locale: 'zhHans',
    messages: { zhHans }
  },
  icons: {
    defaultSet: 'mdi',
    aliases,
    sets: { mdi }
  }
});

createApp(App).use(head).use(router).use(MotionPlugin).use(vuetify).mount('#app');
