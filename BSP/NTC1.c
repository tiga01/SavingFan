
#include "NTC1.h"


//�����
u8 look_up_table(u16 *a,u8 ArrayLong,u16 data)  	//data��ADC��ֵ
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
    return middle ;		//����õ�������±�[middle]
}

//���������ֵ���õ��¶�ֵ
float num_to_temperature(u8 num)
{
    float  data;
    data = 0.5*num-20;	//�����±�õ���Ӧ���¶�ֵ
    return data;				//�õ��¶ȵ�ֵ
}

/*    if( (data<=NTCTAB[0]) && (data>NTCTAB[240]) )
    {
        num=look_up_table(NTCTAB,241,data);
        t1=num_to_temperature(num);
        tx=0.5*(data-NTCTAB[num])/(NTCTAB[num+1]-NTCTAB[num])+t1;
        printf("temp_ntc:%4.2f\r\n",tx);		//0.1%�����
    }
*/
