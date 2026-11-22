/******* Swatch settings *******/
// 5 x 5 grids: lightness from top (lighter) to bottom (darker), changing by `lightness` between rows,
// and `columns` from left to right, changing by `step` between neighbors

// Background colors: plain tiles, to lay on the keys and the panel
const SURFACE_GRIDS = [
  {
    title: 'Function keys (taupe)',
    color: 'taupe',
    lightness: 3,
    columns: 'hue',
    step: 6,
    hint: '← redder · yellower →'
  },
  {
    title: 'Letter keys',
    color: 'key',
    lightness: 3,
    columns: 'hue',
    step: 6,
    hint: '← pinker · browner →'
  },
  {
    title: 'Envoi (green)',
    color: 'green',
    lightness: 3,
    columns: 'saturation',
    step: 8,
    hint: '← greyer · more colorful →'
  },
  {
    title: 'Panel (around the keys)',
    color: 'panel',
    lightness: 3,
    columns: 'saturation',
    step: 3,
    hint: '← greyer · redder →'
  },
];

// Inks: bold text on a fixed background
const INK_GRIDS = [
  {
    title: 'Legend (on letter keys)',
    color: 'legend',
    bg: 'key',
    lightness: 3,
    columns: 'hue',
    step: 8,
    hint: '← warmer · greener →'
  },
  {
    title: 'Fn labels, blue (on panel)',
    color: 'fn',
    bg: 'panel',
    lightness: 6,
    columns: 'saturation',
    step: 12,
    hint: '← greyer · more colorful →'
  },
  {
    title: 'Shift labels, yellow (on panel)',
    color: 'shift',
    bg: 'panel',
    lightness: 5,
    columns: 'saturation',
    step: 10,
    hint: '← greyer · yellower →'
  },
];


/******* Swatches *******/
const cssColor = name => getComputedStyle(document.documentElement).getPropertyValue('--' + name).trim();

function swatch(tile, isCurrent, label) {
    return `<figure class="swatch ${isCurrent ? 'current' : ''}">${tile}<figcaption>${label}</figcaption></figure>`;
}

function block(grid, cells) {
    const hint = '↑ lighter · ↓ darker · ' + grid.hint;
    return `<section><h2>${grid.title}</h2><p class="hint">${hint}</p><div class="swatch-grid">${cells.join('')}</div></section>`;
}

// Calls `cell(color, isCurrent)` for the 25 variations of a grid's color
function variations(grid, cell) {
    const base = cssColor(grid.color);
    const cells = [];
    for (let row = -2; row <= 2; row++) {
        for (let col = -2; col <= 2; col++) {
            const color = vary(base, { lightness: -row * grid.lightness, [grid.columns]: col * grid.step });
            cells.push(cell(color, row === 0 && col === 0));
        }
    }
    return cells;
}

for (const grid of SURFACE_GRIDS) {
    const cells = variations(grid, (color, isCurrent) =>
        swatch(`<div class="swatch-surface" style="--bg: ${color}"></div>`, isCurrent, color));
    document.getElementById('surfaces').insertAdjacentHTML('beforeend', block(grid, cells));
}

for (const grid of INK_GRIDS) {
    const bg = cssColor(grid.bg);
    const cells = variations(grid, (color, isCurrent) =>
        swatch(`<div class="sticker size-regular text-big type-centered" style="--bg: ${bg}; color: ${color}">Aa</div>`, isCurrent, color));
    document.getElementById('inks').insertAdjacentHTML('beforeend', block(grid, cells));
}
