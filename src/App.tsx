import { defineComponent } from 'vue';
import { RouterView } from 'vue-router';
import { ensureDirectory } from './utils';

export default defineComponent({
  setup() {
    import.meta.env.PROD &&
      document.addEventListener('contextmenu', event => event.preventDefault());
    ensureDirectory();
    return () => <RouterView></RouterView>;
  }
});
