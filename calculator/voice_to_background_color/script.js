var body = document.getElementById("body");
var txt = document.getElementById("p");



function listen() {
    // Check if the browser supports the Web Speech API
    if ('SpeechRecognition' in window || 'webkitSpeechRecognition' in window) {
        // Create a new SpeechRecognition object
        var recognition = new (window.SpeechRecognition || window.webkitSpeechRecognition)();

        // Set additional options (optional)
        recognition.lang = 'en-US'; // Specify the language

        // Event handler for when the recognition starts
        recognition.onstart = function () {
            txt.innerHTML = "Voice recognition started";
        };

        // Event handler for when the recognition results are available
        recognition.onresult = function (event) {
            // Get the recognized speech
            var transcript = event.results[0][0].transcript.replace(".", "");
            body.style.backgroundColor = transcript;
            console.log(transcript);

            // Handle the recognized text as needed
            // You might want to perform actions based on the recognized speech
        };

        // Event handler for when the recognition ends
        recognition.onend = function () {
            txt.innerHTML = "Voice recognition ended";
        };

        // Start voice recognition
        recognition.start();
    } else {
        txt.innerHTML = "Speech recognition not supported in this browser.";
    }

}
