## Installation / Compilation
- To use it in your cpp project, include the `seq_reader.h` header file. The file `json.hpp` need to be in the same directory as `seq_reader.h`, but does not need to be explicitely included. 

- The header also contain ability to display the image in cpp using the [Simple DirectMedia Library 2.0 (SDL2)](https://www.libsdl.org/). To make use of it, simply include the SDL header *before* including `seq_reader.h`. You will then also need to link against `-lSDL2`. Consult the documentation of libSLD2 for more information on usage and installation. 

## Documentation
The file `Test.cpp` contains example of usage.
Everything is in a single namespace `seq_reader`, which has a single class `seq` handling the seq file. 

### Functions:
1. `seq_reader::seq File("filename.seq")`: open a seq file with name `filename.seq`. 
2. `File.read_next(img)`: read the next image and put it in the variable `img`, which is overwritten. `img` must be a vector of vector of a numerical type (ie. `std::vector<std::vector<T>>` with `T` being `double`, `int`, `uint16_t` ...
3. `File.read_all(allimg)`: read all the remaining images, `allimg` is overwritten. `allimg` must be a 3D vector (ie. `vector<vector<vector<T>>>`). 
4. `File.read_image(img, n)`: read image number `n`.

If compiled with SDL support:
5. `File.show(img,   int scale=1, std::optional<T> minimum={}, std::optional<T> maximum={}, int delay=0,)`: display an image `img`. All the arguments are optional except for the first one.

- Scale: integer>1, downsample the image (useful if it is too big to fit on your screen). 
- minimum, maximum: modify the scale of value to display, anything <=minimum will be black, anything >= maxium will be white. Note that when calling the function providing these arguments, they need to be put in curly braces. For empty arguments (or default), the color range is calculated from the min and max value on every individual image. 
- delay: delay in ms to wait once an image is shown. 0 is no delay. Negative number waits indefinitely for a user input: pressing any key will resume the program; while closing the window will also resume the program but no more display can happen after this (otherwise the program will crash).



## Json reader acknlowledgment
The json header `json.hpp` is provided by the project by [Nlohmann](https://github.com/nlohmann/json/tree/develop), under the MIT licence. 
