var display_value = document.getElementById("display");

function append_to_display(input) {
    display_value.value += input;


}

function clear_display() {
    display_value.value = "";

}

function calculate() {
    try {
        result = eval(display_value.value)
        display_value.value = result
    }
    catch (error) {
        display_value.value = "Error"
    }


}
