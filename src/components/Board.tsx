import { defineComponent, ref } from 'vue';
import Tile from './Tile';
import styles from './Board.module.scss';
import useGameState from '~/state/game';
import type { Tile as TileType } from '~/types';

export default defineComponent({
  setup() {
    const _rows = ref(15);
    const columns = ref(15);
    const gameState = useGameState();

    async function handleClick(_tile: TileType) {
      // const { row, column, index } = tile;
      // console.log(await invoke('click_tile', { index, row, column }));
    }

    return () => (
      <div aspect-square w="9/10" mx="auto" mt="10">
        {gameState.value.board.map(tile => (
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
            onClick={() => handleClick(tile)}
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
