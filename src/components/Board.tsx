import { Transition, computed, defineComponent, ref } from 'vue';
import { variants } from '@catppuccin/palette';
import Tile from './Tile';
import styles from './Board.module.scss';
import useGameState from '~/state/game';
import type { Tile as TileType } from '~/types';

export default defineComponent({
  setup() {
    const rows = ref(15);
    const columns = ref(15);
    const gameState = useGameState();
    const tileWidth = computed(() => ((100 / columns.value) * 2 * 9) / 10);
    const selectedTiles = computed(() =>
      gameState.value.tiles.filter(tile => tile.selected)
    );

    async function handleClick(tile: TileType) {
      tile.onBuffer = !tile.onBuffer;
      selectedTiles.value.forEach(tile => {
        tile.selected = false;
      });
      tile.selected = true;
    }

    return () => (
      <div relative>
        <div aspect-square w="9/10" mx="auto" mt="1/5">
          {gameState.value.tiles.map(tile => (
            <Transition name="tile-bounce">
              {!tile.dropped ? (
                <div
                  absolute
                  aspect-square
                  grid
                  place-items="center"
                  class={[styles.tile]}
                  style={{
                    width: `${tileWidth.value}%`,
                    zIndex: tile.index + 1000,
                    filter: `grayscale(${tile.exposed ? '0' : '60%'})`,
                    '--tile-transform': !tile.onBuffer
                      ? `translate(${50 * (tile.column - 1)}%, ${
                          50 * (tile.row - 1) - tile.index * 6
                        }%)`
                      : `translate(${50 * (tile.column - 1)}%, ${
                          (rows.value / 2 + 2) * 100
                        }%)`
                  }}
                  onClick={() => handleClick(tile)}
                  key={tile.id}
                >
                  <Tile
                    text={tile.keyword.content}
                    font-size={`${Math.min(
                      150,
                      760 / tile.keyword.content.length
                    )}%`}
                    color={tile.keyword.color}
                    backgroundColor={tile.keyword.backgroundColor}
                    lineColor={
                      tile.selected ? variants.latte.red.hex : undefined
                    }
                    class={[
                      'transition',
                      'duration-100',
                      'h-100/80',
                      'w-100/80'
                    ]}
                  ></Tile>
                </div>
              ) : null}
            </Transition>
          ))}
        </div>
        <div
          absolute
          left="1/29"
          right="1/29"
          style={{
            top: `${tileWidth.value * (rows.value / 2 + 1) + 10}%`,
            height: 0,
            paddingBottom: `${100 / ((rows.value + 2) / 2) + 1}%`
          }}
        ></div>
      </div>
    );
  }
});
