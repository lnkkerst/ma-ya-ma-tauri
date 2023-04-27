import { defineComponent } from 'vue';
import Board from '~/components/Board';
import Buffer from '~/components/Buffer';

export default defineComponent({
  setup() {
    return () => (
      <>
        <Board></Board>
        <Buffer></Buffer>
      </>
    );
  }
});
