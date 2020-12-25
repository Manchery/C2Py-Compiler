int c=0;

int func1(int a){
    c=c+1;
    a=c;
    return a;
}

int main(){
    int a=0;
    a=a+1;
    int a=2;
    b=0;
    a=func1(a);
    b=func2(a);
}