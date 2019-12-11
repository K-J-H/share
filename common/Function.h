#include<stdio.h>

/**
 * 字符串转换数值：
 * 接受的匹配字符串默认是10进制，此外还能接受：
 * 0x表示10进制
 * 0表示8进制
 * b表示2进制
 * 
 * -----version2------
 * 新增符号位判断
*/
int charToint(char* format);


int charToint(char* format){
    int length = 0;
    int sum = 0;
    int sign = 1;
    while(format[length]){
        //printf("format[%d]: %c\n",length,format[length]);
        length ++;
    }
    
    if(format[0] == '-'){
        sign = -1;
        format++;
        length--;
    }

    if(format[0] == '0' && (format[1] == 'x' || format[1] == 'X')){//16进制
        int temp = 2;
        while(temp < length){
            if(format[temp] >= 'a' && format[temp] <= 'f'){
                sum = sum << 4  | (format[temp]-'a'+10);
            }else if(format[temp] >= '0' && format[temp] <= '9'){
                sum = sum << 4 | (format[temp]-'0');
            }else{
                printf("unknown character: %c\n",format[temp]);
                return -1;
            }
            temp ++;
        }
    }else if(format[0] == '0'){//8进制
        int temp = 1;
        while(temp < length){
            if(format[temp] >= '0' && format[temp] <= '7'){
                sum = sum << 3 | (format[temp] - '0');
            }else{
                printf("unknown character: %c\n",format[temp]);
                return -1;
            }
            temp ++;
        }
    }else if(format[0] == 'b' || format[0] == 'B'){//二进制
        int temp = 1;
        while(temp < length){
            if(format[temp] == '0' || format[temp] == '1'){
                sum = sum << 1 |(format[temp] - '0');
            }else{
                printf("unknown character: %c\n",format[temp]);
                return -1;
            }
            temp ++;
        }
    }else{//默认10进制处理

        int temp = 0;
        while(temp < length){
            char c = format[temp];
            if(c >= '0' && c <= '9'){
                sum = sum*10 + c - '0';
            }else{
                printf("unknown character: %c\n",format[temp]);
                return -1;
            }
            temp ++;
        }
    }
    return sign*sum;
}