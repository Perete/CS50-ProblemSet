let isRecording = false

// function startRecording(cam_id) {
function startRecording() {
    // fetch(`/start_recording/${cam_id}`, { method: 'POST' });
    
    fetch(`/start_recording`, { method: 'POST' });
    isRecording = true
    toggleRecordingButtons()
}

// function stopRecording(cam_id) {
function stopRecording() {
    fetch(`/stop_recording`, { method: 'POST' });
    isRecording = false
    toggleRecordingButtons()
}

function toggleRecordingButtons() {
    const startBtn = document.getElementById('startBtn');
    const stopBtn = document.getElementById('stopBtn');

    if (isRecording) {
        startBtn.style.display = 'none'; // Ocultar el botón de inicio
        stopBtn.style.display = 'inline-block'; // Mostrar el botón de detener
    } else {
        startBtn.style.display = 'inline-block'; // Mostrar el botón de inicio
        stopBtn.style.display = 'none'; // Ocultar el botón de detener
    }
}

// Inicializa el estado de los botones al cargar la página
window.onload = function() {
    toggleRecordingButtons();
};

function stopServer() {
    fetch(`/stop_server`, { method: 'POST' })
    .then(response => {
    if (response.ok) {
        alert('El servidor se está cerrando...');
        setTimeout(function() {
            window.close();  // Intenta cerrar la ventana del navegador
        }, 1000);  // Da tiempo para que el servidor se cierre
    } else {
        alert('Hubo un problema al intentar cerrar el servidor.');
    }
});
}

function updateCameraParams() {
fetch('/get_camera_params')
    .then(response => response.text())
    .then(data => {
        const [exposureTimeCam1, analogueGainCam1, exposureTimeCam2, analogueGainCam2] = data.split(',')
        document.getElementById('exposureTimeCam1').innerText = formatNumber(exposureTimeCam1);
        document.getElementById('analogueGainCam1').textContent = formatNumber(analogueGainCam1);
        document.getElementById('exposureTimeCam2').innerText = formatNumber(exposureTimeCam2);
        document.getElementById('analogueGainCam2').textContent = formatNumber(analogueGainCam2);
    })
    .catch(error => console.error('Error:', error));
}

function formatNumber(number) {
    let num = parseFloat(number);
    // Verifica si es un entero
    if (num % 1 === 0) {
        return num.toFixed(0);  // Sin decimales
    } else {
        return num.toFixed(4);  // Con 4 decimales
    }
}

// Actualiza cada 2 segundos
setInterval(updateCameraParams, 2000);

// function setParams(cam_id) {
function setParams() {
    // const gain = document.getElementById(`gain${cam_id}`).value;
    const gain = document.getElementById(`gain`).value;
    const exposure_time = document.getElementById(`exposure_time`).value;
    const formData = new FormData();
    formData.append('gain', gain);
    formData.append('exposure_time', exposure_time);

    fetch(`/set_params`, {
        method: 'POST',
        body: formData
    });
}

function autoAdjust(){
    fetch(`/auto_adjust`, { method: 'POST' });
}