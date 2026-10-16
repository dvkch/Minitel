/******* Spare sticker set *******/
const sheet = document.querySelector('.sheet');
sheet.after(sheet.cloneNode(true));


/******* Icons: give each icon the proportions of its symbol *******/
for (const svg of document.querySelectorAll('svg.icon')) {
    const symbol = document.querySelector(svg.querySelector('use').getAttribute('href'));
    svg.setAttribute('viewBox', symbol.getAttribute('viewBox'));
}
