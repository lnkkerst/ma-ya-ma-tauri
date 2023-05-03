import { defineComponent } from 'vue';
import Board from '~/components/Board';

export default defineComponent({
  setup() {
    return () => (
      <>
        <Board></Board>
      </>
    );
  }
});
