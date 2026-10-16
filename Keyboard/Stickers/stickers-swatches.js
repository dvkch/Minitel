/******* Swatch settings *******/
// 5 x 5 grids of background colors: lightness from top (lighter) to bottom (darker),
// and `columns` from left to right, changing by `step` between neighbors
const LIGHTNESS_STEP = 3;   // %, HSL lightness

const SURFACE_GRIDS = [
  {
    title: 'Function keys (taupe)',
    color: 'taupe',
    columns: 'hue',
    step: 6,
    classes: 'size-medium text-legend ink-legend type-key',
    content: 'Suppr'
  },
  {
    title: 'Letter keys',
    color: 'key',
    columns: 'saturation',
    step: 3,
    classes: 'size-regular text-big ink-legend type-centered',
    content: 'G'
  },
  {
    title: 'Envoi (green)',
    color: 'green',
    columns: 'saturation',
    step: 8,
    classes: 'size-medium text-legend ink-legend type-key',
    content: 'Pg <svg class="icon"><use href="#icon-page-down"/></svg>'
  },
  {
    title: 'Panel (around the keys)',
    color: 'panel',
    columns: 'saturation',
    step: 3,
    classes: 'size-regular text-legend type-key',
    content: '<span class="ink-shift">Tab</span> <span class="ink-fn">F</span>'
  },
];

// 2 x 5 grids of inks: each row changes the ink one way, by `step`, on a given background
const INK_GRIDS = [
  {
    title: 'Legend (on taupe, then on key)',
    color: 'legend',
    classes: 'size-regular text-legend type-key',
    content: 'Ag',
    rows: [
      { bg: 'taupe', vary: 'lightness', step: 6 },
      { bg: 'key', vary: 'lightness', step: 6 }
    ]
  },
  {
    title: 'Fn labels, blue (on panel)',
    color: 'fn',
    classes: 'size-regular text-legend type-fn',
    content: 'Vol +',
    rows: [
      { bg: 'panel', vary: 'lightness', step: 6 },
      { bg: 'panel', vary: 'saturation', step: 12 }
    ]
  },
  {
    title: 'Shift labels, yellow (on panel)',
    color: 'shift',
    classes: 'size-regular text-legend type-shift',
    content: 'Tab',
    rows: [
      { bg: 'panel', vary: 'lightness', step: 6 },
      { bg: 'panel', vary: 'saturation', step: 12 }
    ]
  },
];

const HINTS = {
    hue: '← redder · yellower →',
    saturation: '← greyer · more colorful →',
};


/******* Swatches *******/
const cssColor = name => getComputedStyle(document.documentElement).getPropertyValue('--' + name).trim();

function swatch(classes, content, bg, ink, isCurrent, label) {
    const inkStyle = ink ? `color: ${ink};` : '';
    return `<figure class="swatch ${isCurrent ? 'current' : ''}">
        <div class="sticker ${classes}" style="--bg: ${bg}; ${inkStyle}">${content}</div>
        <figcaption>${label}</figcaption>
    </figure>`;
}

function block(title, hint, cells) {
    return `<section><h2>${title}</h2><p class="hint">${hint}</p><div class="swatch-grid">${cells.join('')}</div></section>`;
}

for (const grid of SURFACE_GRIDS) {
    const base = cssColor(grid.color);
    const cells = [];
    for (let row = -2; row <= 2; row++) {
        for (let col = -2; col <= 2; col++) {
            const color = vary(base, { lightness: -row * LIGHTNESS_STEP, [grid.columns]: col * grid.step });
            cells.push(swatch(grid.classes, grid.content, color, null, row === 0 && col === 0, color));
        }
    }
    const hint = '↑ lighter · ↓ darker · ' + HINTS[grid.columns];
    document.getElementById('surfaces').insertAdjacentHTML('beforeend', block(grid.title, hint, cells));
}

for (const grid of INK_GRIDS) {
    const base = cssColor(grid.color);
    const cells = [];
    for (const row of grid.rows) {
        for (let col = -2; col <= 2; col++) {
            const ink = vary(base, { [row.vary]: col * row.step });
            cells.push(swatch(grid.classes, grid.content, cssColor(row.bg), ink, col === 0, ink));
        }
    }
    const hint = grid.rows.map((row, i) => `row ${i + 1}: ↔ ${row.vary}`).join(' · ');
    document.getElementById('inks').insertAdjacentHTML('beforeend', block(grid.title, hint, cells));
}
