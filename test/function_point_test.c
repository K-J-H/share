#include<stdio.h>

void inner_function(){
    printf("hello\n");
}
int outer_function(void (*inner_function)(),int a){
    inner_function();
    
}

int main(){
    outer_function(inner_function,1);
}

