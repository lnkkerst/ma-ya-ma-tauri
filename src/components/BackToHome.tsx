import { defineComponent } from 'vue';
import { useRouter } from 'vue-router';
import { VBtn } from 'vuetify/components';

export default defineComponent({
  setup() {
    const router = useRouter();
    return () => (
      <div>
        {/* @ts-expect-error: why? */}
        <VBtn icon="mdi-home" variant="text" size="x-large" onClick={() => router.push('/')}></VBtn>
      </div>
    );
  }
});
