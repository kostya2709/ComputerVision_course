import numpy as np
from PIL import Image

def calc_intensity(pixel):
    red, green, blue = pixel
    return 0.299 * red + 0.587 * green + 0.144 * blue

def calculate_psnr( original, result):
    array_1 = np.array( Image.open( original), dtype=np.int64)
    array_2 = np.array( Image.open( result), dtype=np.int64)

    original_intensity = np.apply_along_axis( calc_intensity, -1, array_1)
    result_intensity   = np.apply_along_axis( calc_intensity, -1, array_2)
    mse = np.sum((original_intensity - result_intensity) ** 2) / (array_1.shape[0] * array_2.shape[1])
    return 20 * np.log10(255) - 10 * np.log10(mse)

print( calculate_psnr( "Original.bmp", "result.bmp"))