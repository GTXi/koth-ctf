const REFRESH_INTERVAL = 10;
let countdown = REFRESH_INTERVAL;
const medals = ['🥇', '🥈', '🥉'];

function fetchScores() {
    fetch('/api/scores')
        .then(res => res.json())
        .then(data => {
            renderTable(data);
            countdown = REFRESH_INTERVAL;
        })
        .catch(err => console.error(err));
}

function renderTable(scores) {
    const tbody = document.getElementById('scores-body');
    if (!scores || scores.length === 0) return;
    tbody.innerHTML = scores.map((row, i) => {
        const rank = medals[i] ?? (i + 1);
        return `
        <tr>
            <td class="rank-cell">${rank}</td>
            <td class="user-cell">${row.user}</td>
            <td class="points-cell">${row.points}</td>
            <td class="time-cell">${row.datetime_stamp ?? '—'}</td>
        </tr>`;
    }).join('');
}

setInterval(() => {
    countdown--;
    const el = document.getElementById('countdown');
    if (el) el.textContent = countdown;
    if (countdown <= 0) fetchScores();
}, 1000);
