// Check if the browser supports the Web Speech API
if ('speechSynthesis' in window) {
    // Create a new SpeechSynthesisUtterance object
    var utterance = new SpeechSynthesisUtterance();

    // Set the text you want to convert to speech
    utterance.text = "Hello, this is text-to-speech in the browser.";

    // Use the speechSynthesis object to speak the utterance
    speechSynthesis.speak(utterance);
} else {
    console.error("Speech synthesis not supported in this browser.");
}
