import { defineComponent } from 'vue';
import Board from './Board';

export default defineComponent({
  setup() {
    return () => (
      <div relative>
        <Board></Board>
      </div>
    );
  }
});
