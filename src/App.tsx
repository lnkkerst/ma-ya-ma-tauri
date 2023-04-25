import { defineComponent, reactive, ref } from 'vue';
import { invoke } from '@tauri-apps/api';

export default defineComponent({
  setup() {
    const name = ref('');
    const greetMsg = ref('');
    const addValue = reactive({
      a: '',
      b: '',
      c: ''
    });

    async function handleGreet() {
      greetMsg.value = await invoke('greet', { name: name.value });
    }
    async function handleAdd() {
      addValue.c = await invoke('add', { x: addValue.a, y: addValue.b });
    }

    return () => (
      <div>
        hello world
        <div>
          <input type="text" v-model={name.value} />
          <button onClick={handleGreet}>greet</button>
          <p>{greetMsg.value}</p>
        </div>
        <div>
          <input type="number" v-model={addValue.a} style={{ width: '50px' }} />
          <span>+</span>
          <input type="number" v-model={addValue.b} style={{ width: '50px' }} />
          <span>=</span>
          <input
            type="number"
            disabled
            v-model={addValue.c}
            style={{ width: '50px' }}
          />
          <button onClick={handleAdd}>add</button>
        </div>
      </div>
    );
  }
});
