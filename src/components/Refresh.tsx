import { defineComponent } from 'vue';
import { useRoute, useRouter } from 'vue-router';
import { VBtn } from 'vuetify/components';

export default defineComponent({
  setup() {
    const router = useRouter();
    const route = useRoute();
    return () => (
      <div absolute top="1/100" right="15/100">
        <VBtn
          icon="mdi-refresh"
          variant="text"
          size="x-large"
          onClick={() => location.reload()}
        ></VBtn>
      </div>
    );
  }
});
