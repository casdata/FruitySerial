# Fruity Serial Terminal
Serial terminal with a colorful feature.

![Screenshot from 2024-04-15 15-11-40](https://github.com/casdata/FruitySerial/assets/31713203/56d114c5-6218-471e-a435-cdddcad8d2ce)
![Screenshot from 2024-04-15 15-10-56](https://github.com/casdata/FruitySerial/assets/31713203/ae6618c5-a59b-4af9-a8e5-7260728281c7)
![Screenshot from 2024-04-15 15-10-37](https://github.com/casdata/FruitySerial/assets/31713203/4abfdc4d-c10f-4f3a-a6a2-a6b17dc4cdba)



## Colors
The data received can be displayed in colors for a better display of information, giving a more personalized and fun feeling.

- Set text color by name: [t color_name] 
            E.g. [t red] [t blue] [t white]
  
- Set text by hex value: [t #RGB] [t #RRGGBB]
  E.g. [t #f81] [t #ff10bd] 

- Reset text color: [t]

#### Example
"Hello friend, [t red]this is a [t][t green]test[t], but you can make your own [t #0066ff]customization[t]."
$${Hello \space friend, \space \color{red}this \space is \space a \space \color{green}test \color{white}. \space but \space you 
\space can \space make \space your \space own \space \color{#0066ff}customization \space \color{white}.}$$

#### Available Colors by name
- Blue
- Red
- Green
- Purple
- Yellow
- Silver
- Gray
- Maroon
- Alive
- Lime
- Aqua
- Teal
- Navy
- Fuchsia
- Black
- White

## Installation
For linux its required to install the sdl2-image library.
- Ubuntu: sudo apt install libsdl2-image-2.0-0
- Arch:   sudo pacman -S sdl2_image
