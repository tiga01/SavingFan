
#include "NTC1.h"


//查表函数
u8 look_up_table(u16 *a,u8 ArrayLong,u16 data)  	//data：ADC的值
{
    u16 begin,end,middle ;
    u8 i ;
    begin = 0 ;
    end = ArrayLong-1 ;
    i = 0  ;
    if(data >= a[begin]) return begin ;
    else if(data <= a[end]) return end ;
    while(begin < end)
    {
            middle = (begin+end)/2 ;
            if(data == a[middle] ) break ;
            if(data < a[middle] && data > a[middle+1]) break ;
            if(data > a[middle])  end = middle ;
            else begin = middle ;
            if(i++ > ArrayLong) break ;
    }
    if(begin > end ) return 0 ;
    return middle ;		//查表后得到数组的下标[middle]
}

//输入表的序号值，得到温度值
float num_to_temperature(u8 num)
{
    float  data;
    data = 0.5*num-20;	//根据下标得到对应的温度值
    return data;				//得到温度的值
}

/*    if( (data<=NTCTAB[0]) && (data>NTCTAB[240]) )
    {
        num=look_up_table(NTCTAB,241,data);
        t1=num_to_temperature(num);
        tx=0.5*(data-NTCTAB[num])/(NTCTAB[num+1]-NTCTAB[num])+t1;
        printf("temp_ntc:%4.2f\r\n",tx);		//0.1%的误差
    }
*/
