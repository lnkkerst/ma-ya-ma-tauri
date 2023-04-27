import { variants } from '@catppuccin/palette';
import { defineComponent } from 'vue';
import { useAppConfig } from '~/state/config';

export default defineComponent({
  props: {
    text: {
      type: String
    },
    color: {
      type: String,
      default: variants.latte.text.hex
    },
    backgroundColor: {
      type: String,
      default: variants.latte.base.hex
    }
  },
  setup(props) {
    const appConfig = useAppConfig();
    return () => (
      <svg
        xmlns="http://www.w3.org/2000/svg"
        viewBox="0 0 100 100"
        cursor="default"
      >
        <rect
          x="10"
          y="15"
          width="80"
          height="80"
          rx="8"
          fill={variants.latte.surface2.hex}
          stroke={variants.mocha.crust.hex}
          stroke-width="3"
        ></rect>
        <rect
          x="10"
          y="10"
          width="80"
          height="80"
          rx="8"
          fill={props.backgroundColor}
          stroke={variants.mocha.crust.hex}
          stroke-width="3"
        ></rect>
        <text
          x="50%"
          y="50%"
          text-anchor="middle"
          dominant-baseline="middle"
          stroke={props.color}
          fill={props.color}
          class={[
            {
              'font-jetbrains-mono':
                appConfig.value.tileFont === 'jetbrains-mono',
              'font-fira-code': appConfig.value.tileFont === 'fira-code'
            }
          ]}
        >
          {props.text}
        </text>
      </svg>
    );
  }
});
