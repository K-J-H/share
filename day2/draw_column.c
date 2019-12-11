#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * the first appearen to paint a picture is specifing the device file
 * wihch is specify the whole pixel[row_pixel][column_pixel], when each row pixel if the function of row index:
 * when row=0, pixel[0][column] = function(column) e.g. for(i:column){pixel[0][i] = row0function(i)}
 * when row=1, pixel[1][column] = function(column) e.g. for(i:column){pixel[1][i] = row1function(i)}
 *                                                 ...
 *                                                 ...
 *                                                 ...
 * when row=row_piexl, pixel[row_pixel][column] = function(column) e.g. for(i:column){pixel[row_pixel][i] = rownfunction(i)}
 * 
 * for each row, there is a rowfuncion to specify that every pixel in that row, let's say function(i:)
 * so if there's a funciton, the generou perpous loop is like:
 * 
 * for(int i = 0; i < row_pixel; i ++){
 *      for(int j = 0; j < column_pixel; j ++){
 *          pixel[i][j] = row_(i)_function(j)
 *      }
 * }
 * write(fd, p, row_pixel*column_pixel*sizeof(int))
 * 
 * 
 * if write one row per time, which means declare int *[row_pixel] other than int *[row_pixel][column_pixel]
 * the general paint is just like:
 * #define onepixelsize type
 * type p[row_pixel];
 * int fd = open(devpath,read&write);
 * 
 * specify row1 pixel:
 * p[row_pixel] = function_row(0)
 * write(fd, p, row_pixel*sizeof(int))
 *          ...
 *          ...
 *          ...
 * specify rown pixel:
 * p[row_pixel] = function_row(n)
 * write(fd, p, row_pixel*sizeof(int))
 * 
 * so the loop is just like this:
 * for(int i = 0; i < row_pixel; i++){
 *      specify rowi pixel:
 *      pixel[row_pixel] = function_(i)_
 *      write(fd, p, row_pixel*sizeof(int))
 * }
 * 
 * the toppest level is like: draw(int p[row_pixel], row_pixel, column_pixel)
*/

                                         
