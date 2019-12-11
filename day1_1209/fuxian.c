/**
 * 转场效果之一：
 * 浮现。
 * 假设X旧Y旧，如何逐渐变成X新Y新？
 * 假设速度要为v，则必须由对应的要draw的帧数p
 * 是否需要每个点都计算？
 * Xn=X旧+n(X新-X旧)/p；0<n<p
*/
read("/dev/fb0",buf_old[column][row])
buf_new[column_new][row_old];
difference[][];
for(int i = 0; i < p; i ++){
    for(j: column_new){
        for(k: row_new){
            *(mmap_lcd + k*row+j) += difference[i][j]   
        }
    }
    sleep();
}