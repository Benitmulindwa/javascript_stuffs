var mydiv = document.getElementById("mydiv");

            var div1 = document.getElementById("div1");
            var div2 = document.getElementById("div2");
            var div3 = document.getElementById("div3");



            function changeColor() {
                const pos = ["center", "space-between", "space-around", "space-evenly", "flex-end", "flex-start"];
                const color = [
                    "Red",
                    "Green",
                    "Blue",
                    'Cyan',
                    "Magenta",
                    "Yellow",
                    "Black",
                    "White",
                    "Gray",
                    "Silver",
                    "Maroon",
                    "Olive",
                    "Navy",
                    "Purple",
                    "Teal",
                    "Orange",
                    "Pink",
                    "Brown",
                    "Coral",
                    "Gold"
                ];
                const random_pos = Math.floor(Math.random() * pos.length);
                const random_color = Math.floor(Math.random() * color.length);

                mydiv.style.justifyContent = pos[random_pos];


                mydiv.style.backgroundColor = color[random_color];


                div1.style.backgroundColor = color[random_color + 1];
                div2.style.backgroundColor = color[random_color - 1];
                div3.style.backgroundColor = color[random_color - 2];




            }
            setInterval(changeColor, 400);
