import { defineComponent, onMounted, ref } from 'vue';
import { useRoute } from 'vue-router';
import { VCard, VTable } from 'vuetify/components';
import BackToHome from '~/components/BackToHome';
import useWindowScale from '~/composables/windowScale';
import type { GameRecord } from '~/types';
import { readRecords } from '~/utils';

export default defineComponent({
  setup() {
    const route = useRoute();
    const scale = useWindowScale();
    const levelName = (route.query.level as string) ?? '1';
    const records = ref<GameRecord[]>([]);
    onMounted(async () => {
      records.value = await readRecords(levelName);
    });
    return () => (
      <div>
        <BackToHome></BackToHome>
        <div
          text-center
          font-bold
          style={{
            marginTop: `${scale.value * 64}px`,
            fontSize: `${scale.value * 32}px`
          }}
        >{`关卡：${levelName}`}</div>
        <div
          style={{
            padding: `${scale.value * 24}px ${scale.value * 64}px`
          }}
        >
          <VCard>
            <VTable height={scale.value * 768} fixedHeader>
              <thead>
                <tr>
                  <th>昵称</th>
                  <th>分数</th>
                </tr>
              </thead>
              <tbody>
                {records.value.map(record => (
                  <tr>
                    <td>{record.name}</td>
                    <td>{record.score}</td>
                  </tr>
                ))}
              </tbody>
            </VTable>
          </VCard>
        </div>
      </div>
    );
  }
});
