import { defineComponent } from 'vue';
import { useRouter } from 'vue-router';
import { VBtn } from 'vuetify/components';

export default defineComponent({
  setup() {
    const router = useRouter();
    return () => (
      <div>
        <div py="1/5" text="5vh" text-center>
          码呀码
        </div>
        <div flex flex-column items-center justify-center w-full>
          {/* @ts-expect-error: why? */}
          <VBtn onClick={() => router.push('levels')}>开始游戏</VBtn>
        </div>
      </div>
    );
  }
});
