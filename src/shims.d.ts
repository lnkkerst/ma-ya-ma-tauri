import type { AttributifyAttributes } from 'unocss/preset-attributify';

type ExtraUnocssAttributifyName = 'mx' | 'my' | 'px' | 'py' | 'duration' | 'transition' | 'left' | 'right' | 'top' | 'bottom';

declare module '@vue/runtime-dom' {
  interface HTMLAttributes extends AttributifyAttributes {}
  interface HTMLAttributes extends Partial<Record<ExtraUnocssAttributifyName, string | boolean>> {}
  interface SVGAttributes extends AttributifyAttributes {}
}

declare module 'canvas-nest.js' {}
