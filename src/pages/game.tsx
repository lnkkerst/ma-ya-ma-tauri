import { invoke } from '@tauri-apps/api';
import { defineComponent, ref } from 'vue';
import { useRoute, useRouter } from 'vue-router';
import { gsap } from 'gsap';
import { VBtn, VCard, VCardActions, VCardText, VCardTitle, VDialog, VTextField } from 'vuetify/components';
import Board from '~/components/Board';
import useGameState from '~/state/game';
import type { Tile } from '~/types';
import { GameStatus } from '~/types';

export default defineComponent({
  async setup() {
    const route = useRoute();
    const router = useRouter();
    const levelName = route.query.level || '1';
    const gameState = useGameState();
    const scoreEl = ref<HTMLDivElement | null>(null);
    const endDialog = ref(false);
    const name = ref({ content: '', rules: [(v: string) => !!v || '必须提供名字'] });

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

    return () => (
      <div>
        <div absolute top="1/50" right="1/20">
          {/* @ts-expect-error: why? */}
          <VBtn icon="mdi-home" variant="text" onClick={() => router.push('/')}></VBtn>
        </div>
        <div absolute cursor="default" top="2/100" left="4/100" ref={scoreEl} font="bold" style={{ fontSize: '2vh' }}>{`得分：${gameState.value.score}`}</div>
        <VDialog v-model={endDialog.value} select-none persistent>
          {
            {
              [GameStatus.Losed]: (
                <VCard>
                  <VCardTitle class={['mx-auto']} cursor-default>
                    你输了喵 😜
                  </VCardTitle>
                  <VCardText cursor-default mx-auto>
                    {`最终得分 ${gameState.value.score} 分，然而并没有什么卵用 😢`}
                  </VCardText>
                  <VCardActions mx-auto>
                    {/* @ts-expect-error: why? */}
                    <VBtn variant="tonal" onClick={() => router.push('/')}>
                      返回主菜单
                    </VBtn>
                    {/* @ts-expect-error: why? */}
                    <VBtn variant="tonal" onClick={() => location.reload()}>
                      再来一次
                    </VBtn>
                  </VCardActions>
                </VCard>
              ),
              [GameStatus.Winned]: (
                <VCard>
                  <VCardTitle class={['mx-auto']}>你赢了！🎉 </VCardTitle>
                  <VCardText mx-auto>
                    <p>{`最终得分：${gameState.value.score}`}</p>
                    <div>
                      <VTextField v-model={name.value.content} label="输入昵称 😎" rules={name.value.rules}></VTextField>
                      {/* @ts-expect-error: why? */}
                      <VBtn onClick={handleSaveScore}>保存成绩</VBtn>
                    </div>
                  </VCardText>
                </VCard>
              ),
              [GameStatus.Running]: (
                <VCard>
                  <VCardTitle class={['mx-auto']}>你好像来了不该来的地方 😿</VCardTitle>
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
