/**
 * 电子相册：
 * 打开电子相册的程序，首先出现第一张默认出现的图片（或可以在打开时指定）
 * 将屏幕分成两个部分，点击左或者右，有相应的图片出现，图片大小不一定时全屏大小，
 * 而且每个图片都由出场方式
 * 大概思路：假设bitmap文件已经存放在一个数组中
 * open default photo;
 * getpoint(x,y)
 * if(x < 400 )
 *      path = bimap_list.next->path;
 * else
 *      path = bitmap_list.fron->path;
 * darw bitmap(path, 转场效果)
 * goto getpoint
*/
