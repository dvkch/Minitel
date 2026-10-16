# Minitel/Keyboard/Stickers

The Minitel keyboard drivers in this repo change the meaning of some keys to remove Minitel keys that don't have any use on a standard PC keyboard, and add back useful PC keys.

In this folder you can print stickers to update key labels to match. They are designed to be printed on white matte inkjet vinyl.

### Printing

Open `stickers.html` in a recent Chrome or Safari. It has two A4 pages:

- page 1: color swatches, 5 × 5 variations around each color
- page 2: every sticker at its exact size, with 1 mm bleed and crop marks, in two identical sets

Print with margins "None", scale 100 % and background graphics on. The line at the top of each page must measure exactly 10 cm. Sizes are accurate to about ±0.2 mm: browsers round some drawing to their 0.26 mm pixel grid.

### Picking the colors

1. Print page 1 on the vinyl (colors depend on the paper), let it dry.
2. In daylight, lay the swatches on the keys and the panel.
3. Copy the best hex into the colors at the top of `stickers-variables.css`, reload. The grids re-center on the new values; lower the `step` values in `stickers-swatches.js` for a finer second round.
4. Print page 2 on the vinyl.

### Dimensions

Measured on a Minitel 1B (RTIC), with a caliper and a 600 dpi scan:

- Keys: 
  - Top row (function keys): 19.6 × 7.7 mm
  - Space: 75.3 × 7.7 mm
  - All other keys: 11.7 × 7.7 mm. 
- Ctrl and Shift frames: 
  - 14.8 × 10.8 mm outside
  - 0.5 mm line.
- Legends: 
  - Font: Barlow Regular (SIL Open Font License, in `fonts/`), tracking -60. Icons use Barlow's stem width.
  - Top Row:
    - 2.6 mm cap height,
    - left-aligned 1.8 mm from the key's left edge,
    - cap top 1.27 mm from its top edge.
  - Letters: 
    - 5.0 mm
    - centered
  - Panel legends: 
    - about 2.6 mm height
    - ending ~1.4 mm above the key
    - Shift: 
      - left-aligned with the key's left edge. 
      - Ctrl: right-aligned with its right edge. 
      - The new Fn labels follow the Ctrl rule, 
      - the new Shift ones the Shift rule.
- Stickers are 0.5 mm smaller on each side.
- Scanned colors: taupe `#63524B`, green `#55623D`, keys `#36302F`, panel `#363131`, legends `#B0AEA4`.

When a new label lands on an old one (Brk, Œ, ß, ↑ on keypad 0), its sticker is sized to cover it.
