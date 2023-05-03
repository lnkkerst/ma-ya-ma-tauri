import type { AttributifyAttributes } from 'unocss/preset-attributify';

type ExtraUnocssAttributifyName =
  | 'mx'
  | 'my'
  | 'duration'
  | 'transition'
  | 'left'
  | 'right';

declare module '@vue/runtime-dom' {
  interface HTMLAttributes extends AttributifyAttributes {}
  interface HTMLAttributes
    extends Partial<Record<ExtraUnocssAttributifyName, string | boolean>> {}
  interface SVGAttributes extends AttributifyAttributes {}
}

declare module 'canvas-nest.js' {}
