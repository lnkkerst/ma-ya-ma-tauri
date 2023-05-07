import { invoke } from '@tauri-apps/api';
import { defineComponent, ref } from 'vue';
import { useRoute, useRouter } from 'vue-router';
import { gsap } from 'gsap';
import {
  VBtn,
  VCard,
  VCardActions,
  VCardText,
  VCardTitle,
  VDialog,
  VTextField
} from 'vuetify/components';
import Board from '~/components/Board';
import useGameState from '~/state/game';
import type { Tile } from '~/types';
import { GameStatus } from '~/types';
import BackToHome from '~/components/BackToHome';
import { addRecord } from '~/utils';
import useWindowScale from '~/composables/windowScale';
import Refresh from '~/components/Refresh';

export default defineComponent({
  async setup() {
    const route = useRoute();
    const router = useRouter();
    const scale = useWindowScale();
    const levelName = (route.query.level as string) || '1';
    const gameState = useGameState();
    const scoreEl = ref<HTMLDivElement | null>(null);
    const endDialog = ref(false);
    const name = ref({
      content: '',
      rules: [(v: string) => !!v || '必须提供名字']
    });
    const loading = ref(false);

    await invoke('load_theme_from_builtin', { themeName: 'default' });
    gameState.value.tiles = (await invoke('load_level_from_builtin', {
      levelName
    })) as Tile[];
    gameState.value.tiles.forEach(tile => {
      gameState.value.tilesMap[tile.id] = tile;
    });

    async function handleClickTile() {
      setTimeout(updateScore, 350);
      const status = (await invoke('get_status')) as { repr: number };
      gameState.value.status = status.repr;
      if (gameState.value.status !== GameStatus.Running) {
        setTimeout(() => (endDialog.value = true), 300);
      }
    }

    async function updateScore() {
      const newScore = (await invoke('get_score')) as number;
      if (gameState.value.score !== newScore) {
        gameState.value.score = newScore;
        if (!scoreEl.value) {
          return;
        }
        gsap.to(scoreEl.value, {
          scale: 1.1,
          repeat: 1,
          duration: 0.2,
          yoyo: true
        });
      }
    }

    async function handleAddRecord() {
      if (!name.value.content) {
        return;
      }
      loading.value = true;
      await addRecord(levelName, {
        name: name.value.content,
        score: gameState.value.score
      });
      loading.value = false;
      router.push({ path: '/records', query: { level: levelName } });
    }

    return () => (
      <div>
        <BackToHome></BackToHome>
        <Refresh></Refresh>
        <div
          absolute
          cursor="default"
          top="2/100"
          left="4/100"
          ref={scoreEl}
          font="bold"
          style={{ fontSize: '2vh' }}
        >{`得分：${gameState.value.score}`}</div>
        <VDialog v-model={endDialog.value} select-none persistent>
          {
            {
              [GameStatus.Losed]: (
                <VCard style={{ maxWidth: `${scale.value * 480}px` }} mx-auto>
                  <VCardTitle class={['mx-auto']} cursor-default>
                    你输了喵 😜
                  </VCardTitle>
                  <VCardText cursor-default mx-auto>
                    {`最终得分 ${gameState.value.score} 分，然而并没有什么卵用 😢`}
                  </VCardText>
                  <VCardActions mx-auto>
                    <VBtn variant="tonal" onClick={() => router.push('/')}>
                      返回主菜单
                    </VBtn>
                    <VBtn variant="tonal" onClick={() => location.reload()}>
                      再来一次
                    </VBtn>
                  </VCardActions>
                </VCard>
              ),
              [GameStatus.Winned]: (
                <VCard style={{ maxWidth: `${scale.value * 480}px` }} mx-auto>
                  <VCardTitle class={['mx-auto']}>你赢了！🎉 </VCardTitle>
                  <VCardText mx-auto>
                    <p my-1>{`最终得分：${gameState.value.score}`}</p>
                    <div>
                      <VTextField
                        v-model={name.value.content}
                        variant="outlined"
                        density="compact"
                        label="输入昵称 😎"
                        rules={name.value.rules}
                      ></VTextField>
                      <VBtn
                        variant="tonal"
                        loading={loading.value}
                        onClick={handleAddRecord}
                      >
                        保存成绩
                      </VBtn>
                    </div>
                  </VCardText>
                </VCard>
              ),
              [GameStatus.Running]: (
                <VCard style={{ maxWidth: `${scale.value * 480}px` }} mx-auto>
                  <VCardTitle class={['mx-auto']}>
                    你好像来了不该来的地方 😿
                  </VCardTitle>
                  <VCardText></VCardText>
                </VCard>
              )
            }[gameState.value.status]
          }
        </VDialog>
        <Board onClickTile={handleClickTile}></Board>
      </div>
    );
  }
});
