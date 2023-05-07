import { defineComponent } from 'vue';
import { RouterView } from 'vue-router';
import { ensureDirectory } from './utils';

export default defineComponent({
  setup() {
    ensureDirectory();
    return () => <RouterView></RouterView>;
  }
});
