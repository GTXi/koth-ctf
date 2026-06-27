const REFRESH_INTERVAL = 10;
let countdown = REFRESH_INTERVAL;
const medals = ['🥇', '🥈', '🥉'];
let scoreChart = null;

function initChart() {
    const ctx = document.getElementById('scoreChart').getContext('2d');
    Chart.defaults.color = '#e2e2f0';
    scoreChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: [],
            datasets: [{
                label: 'Blocks Mined',
                data: [],
                backgroundColor: '#a855f7',
                borderRadius: 4
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            scales: { y: { beginAtZero: true, ticks: { stepSize: 1 } } },
            plugins: { legend: { display: false } },
            animation: { duration: 500 }
        }
    });
}

function fetchScores() {
    fetch('/api/scores')
        .then(res => res.json())
        .then(data => {
            renderTable(data);
            updateChart(data);
            countdown = REFRESH_INTERVAL;
        })
        .catch(err => console.error(err));
}

function updateChart(scores) {
    if (!scoreChart) initChart();
    scoreChart.data.labels = scores.map(s => s.user);
    scoreChart.data.datasets[0].data = scores.map(s => s.points);
    scoreChart.update();
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

// Initial fetch immediately on load
fetchScores();

setInterval(() => {
    countdown--;
    const el = document.getElementById('countdown');
    if (el) el.textContent = countdown;
    if (countdown <= 0) fetchScores();
}, 1000);
