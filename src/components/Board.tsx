import { defineComponent, reactive, ref } from 'vue';
import { variants } from '@catppuccin/palette';
import Tile from './Tile';
import styles from './Board.module.scss';
import type { Tile as TileType } from '~/types';

export default defineComponent({
  setup() {
    const rows = ref(15);
    const columns = ref(15);
    const tiles = reactive<TileType[]>([
      {
        row: 1,
        column: 1,
        index: 0,
        exposed: true,
        keyword: {
          content: 'let',
          color: variants.latte.mauve.hex
        }
      },
      {
        row: 1,
        column: 1,
        index: 1,
        exposed: true,
        keyword: {
          content: 'mut',
          color: variants.latte.mauve.hex
        }
      }
    ]);

    return () => (
      <div aspect-square w="9/10" mx="auto" mt="10">
        {tiles.map(tile => (
          <div
            absolute
            aspect-square
            grid
            place-items="center"
            class={[styles.tile]}
            style={{
              width: `${((100 / columns.value) * 2 * 9) / 10}%`,
              zIndex: tile.index + 1000,
              filter: `grayscale(${tile.exposed ? '0' : '60%'})`,
              '--tile-transform': `translate(${50 * (tile.column - 1)}%, ${
                50 * (tile.row - 1) - tile.index * 6
              }%)`
            }}
          >
            <Tile
              text={tile.keyword.content}
              font-size={`${Math.min(150, 760 / tile.keyword.content.length)}%`}
              color={tile.keyword.color}
              backgroundColor={tile.keyword.backgroundColor}
              transition
              duration="100"
              h="100/80"
              w="100/80"
            ></Tile>
          </div>
        ))}
      </div>
    );
  }
});
