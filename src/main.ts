import { createApp } from 'vue';
import { createHead } from '@vueuse/head';
import { MotionPlugin } from '@vueuse/motion';
import { createRouter, createWebHashHistory } from 'vue-router';

import '@unocss/reset/tailwind.css';

import 'virtual:uno.css';
import '~/styles/main.scss';

import generatedRoutes from 'virtual:generated-pages';
import { setupLayouts } from 'virtual:generated-layouts';
import App from './App';

const routes = setupLayouts(generatedRoutes);
const head = createHead();
const router = createRouter({
  routes,
  history: createWebHashHistory()
});

createApp(App).use(head).use(router).use(MotionPlugin).mount('#app');
