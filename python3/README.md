## Usage: 
```python
import seq_reader
[I, metadata] = seq_reader.load_seq(filename)
```

The variable `I` contains the images, and `metadata` is a structure with the content of the log file. Note that `I` is a memory mapped three-dimensional array, so the data are not loaded in memory until needed. However, please note that some operation down the line may load the whole file at once without warning, and may result in crashing if the available memory is exceeded. Operations on the whole array `I` are therefore discouraged for larger files. Prefer looping over the individual images. 
