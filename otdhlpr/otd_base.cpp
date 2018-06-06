#include <otdhlpr/otd_base.h>

div_t  fast_divP2(uint32_t value,uint32_t denom)
{
  div_t ret ;
  ret.rem = static_cast<int>( value&(denom-1));
  if(value>=denom)
      ret.quot = value>>3;
     else
      ret.quot = 0;
  return ret;
}


uint32_t  _get_bit_value(uint8_t* uint8_ts,uint32_t idx)
{
 div_t dt = fast_divP2(idx,8);
 uint8_ts+=dt.quot;
 uint32_t ret = ((*uint8_ts)&(0x80>>dt.rem)) ? 1:0;
 return ret;
}

void   _set_bit_value(uint8_t* ts_array,uint32_t idx,uint32_t value)
{
 div_t dt  = fast_divP2(idx,8);
 ts_array+=dt.quot;
 uint8_t mask = 0x80>>dt.rem;
 if(value)
    (*ts_array)|=mask;
    else
    (*ts_array)&= ~mask;
}




#define GET_REAL_INDEX(n,i) (i-(n)->loN)

//Получение/Установка значения
KERTL_API int   otd_get_value(lpotd_data array,uint8_t number,Pvoid value,size_t sz)
{
  if(array && otd_number_check(&array->numbers,number) && value && sz)
  {
   memset(value,0,sz);
   uint32_t real_idx = GET_REAL_INDEX(&array->numbers,number);
   if(array->otd_type == OTD_DISCRETE)
    {
     uint32_t v = _get_bit_value  (array->data,real_idx);
     memcpy(value,&v,std::min(sz,sizeof(v)));
    }
    else
    {
      size_t native_size = static_cast<size_t>(otd_data_calc_size(array->otd_type,1));
      sz = std::min(sz,native_size);
      memcpy(value,array->data+(native_size*real_idx),sz);
    }
      return 0;
  }
  return -1;
}

KERTL_API int   otd_set_value(lpotd_data array,uint8_t number,Pvoid value,size_t sz)
{
   int ret = -1;
   if(array && otd_number_check(&array->numbers,number) && sz)
   {
    ret=0;
    uint32_t real_idx = GET_REAL_INDEX(&array->numbers,number);
    if(array->otd_type == OTD_DISCRETE)
       _set_bit_value  (array->data,real_idx,*static_cast<uint8_t*>(value));
    else
    {
      size_t native_size = static_cast<size_t>(otd_data_calc_size(array->otd_type,1));
      sz = std::min(sz,native_size);
      memcpy(array->data+(native_size*real_idx),value,sz);

    }
   }
   return ret;
}

template <typename T>
inline void _do_inc_value ( T* ptr,unsigned offset,bool no_zero)
{
  ptr+=offset;
  if(++(*ptr) == T(0) && no_zero) ++(*ptr);
}

inline void _do_inc_value ( float* ptr,unsigned offset,bool no_zero)
{
    ptr+=offset;
    if(fabs(++(*ptr) - float(0)) > float(.00001) && no_zero) ++(*ptr);

}

inline void _do_inc_value ( double* ptr,unsigned offset,bool no_zero)
{
    ptr+=offset;
    if(fabs(++(*ptr) - double(0)) > double(.00001) && no_zero) ++(*ptr);

}


KERTL_API int otd_inc_value(lpotd_data array,uint8_t number,bool no_zero)
{
 int ret = -1;
 if(array && otd_number_check(&array->numbers,number))
   {
     if(array->otd_type&OTD_BIT_TYPE)
     {
      bool v;
      otd_get_value(array,number,&v,sizeof(v));
      if(no_zero || !v)
         v=!v;
      ret = otd_set_value(array,number,&v,sizeof(v));
     }
     else
     {
      unsigned  real_idx = GET_REAL_INDEX(&array->numbers,number);
      switch(array->otd_type)
      {
       case OTD_ANALOG_BYTE    :{_do_inc_value(array->data,real_idx,no_zero); };break;
       case OTD_ANALOG_WORD    :{_do_inc_value(reinterpret_cast<uint16_t*>(array->data),real_idx,no_zero);};break;
       case OTD_ANALOG_SHORT   :{_do_inc_value(reinterpret_cast< int16_t*>(array->data),real_idx,no_zero);};break;
       case OTD_ANALOG_DWORD   :{_do_inc_value(reinterpret_cast<uint32_t*>(array->data),real_idx,no_zero);};break;
       case OTD_ANALOG_LONG    :{_do_inc_value(reinterpret_cast< int32_t*>(array->data),real_idx,no_zero);};break;
       case OTD_ANALOG_QWORD   :{_do_inc_value(reinterpret_cast<uint64_t*>(array->data),real_idx,no_zero);};break;
       case OTD_ANALOG_LONGLONG:{_do_inc_value(reinterpret_cast< int64_t*>(array->data),real_idx,no_zero);};break;
       case OTD_FLOAT          :
       case OTD_SIGNED_FLOAT   :{_do_inc_value(reinterpret_cast< float*>(array->data),real_idx,no_zero);};break;
       case OTD_DOUBLE         :
       case OTD_SIGNED_DOUBLE  :{_do_inc_value(reinterpret_cast<double*>(array->data),real_idx,no_zero);};break;
      }
      ret = 0;
     }
   }
 return ret;
}

template <typename T>
inline void _do_dec_value ( T* ptr,unsigned offset,bool no_zero)
{
  ptr+=offset;
  if(--(*ptr) == T(0) && no_zero) --(*ptr);
}

inline void _do_dec_value ( float* ptr,unsigned offset,bool no_zero)
{
    ptr+=offset;
    if(fabs(--(*ptr) - float(0)) > float(.00001) && no_zero) --(*ptr);

}

inline void _do_dec_value ( double* ptr,unsigned offset,bool no_zero)
{
    ptr+=offset;
    if(fabs(--(*ptr) - double(0)) > double(.00001) && no_zero) --(*ptr);

}


KERTL_API int  otd_dec_value(lpotd_data array,uint8_t number,bool no_zero)
{

 int ret = -1;
 if(array && otd_number_check(&array->numbers,number))
   {
     if(array->otd_type&OTD_BIT_TYPE)
     {
      bool v;
      otd_get_value(array,number,&v,sizeof(v));
      v=!v;
      ret = otd_set_value(array,number,&v,sizeof(v));
     }
     else
     {
         unsigned  real_idx = GET_REAL_INDEX(&array->numbers,number);
         switch(array->otd_type)
         {
          case OTD_ANALOG_BYTE    :{_do_dec_value(array->data,real_idx,no_zero); };break;
          case OTD_ANALOG_WORD    :{_do_dec_value(reinterpret_cast<uint16_t*>(array->data),real_idx,no_zero);};break;
          case OTD_ANALOG_SHORT   :{_do_dec_value(reinterpret_cast< int16_t*>(array->data),real_idx,no_zero);};break;
          case OTD_ANALOG_DWORD   :{_do_dec_value(reinterpret_cast<uint32_t*>(array->data),real_idx,no_zero);};break;
          case OTD_ANALOG_LONG    :{_do_dec_value(reinterpret_cast< int32_t*>(array->data),real_idx,no_zero);};break;
          case OTD_ANALOG_QWORD   :{_do_dec_value(reinterpret_cast<uint64_t*>(array->data),real_idx,no_zero);};break;
          case OTD_ANALOG_LONGLONG:{_do_dec_value(reinterpret_cast< int64_t*>(array->data),real_idx,no_zero);};break;
          case OTD_FLOAT          :
          case OTD_SIGNED_FLOAT   :{_do_dec_value(reinterpret_cast< float*>(array->data),real_idx,no_zero);};break;
          case OTD_DOUBLE         :
          case OTD_SIGNED_DOUBLE  :{_do_dec_value(reinterpret_cast<double*>(array->data),real_idx,no_zero);};break;
         }
      ret = 0;
     }
   }
 return ret;
}


KERTL_API unsigned   otd_copy     (lpotd_data dest,lpotd_data src,uint8_t start,uint8_t count)
{
 //Копирование из одного массива в другой
 unsigned copy_count(0);
 if(dest && src)
 {
 count+=start;
 unsigned src_sz = otd_data_calc_size(src->otd_type,1);
 unsigned dst_sz = otd_data_calc_size(dest->otd_type,1);
 char buf[1024];
 Pvoid value;
 if(dst_sz>sizeof(buf) || src_sz>sizeof(buf))
   value = new char[std::max(dst_sz,src_sz)];
   else
   value = buf;

 for(uint8_t i = start;i<count;i++)
 {
   if(!otd_get_value(src,i,value,src_sz) && !otd_set_value(dest,i,value,dst_sz))
      copy_count++;

 }
 if(value && value!=buf)
   delete [] reinterpret_cast<char*>( value);
 }

 return copy_count;
}


KERTL_API void  otd_clear    (lpotd_data data)
{
 uint64_t v = 0;
 Pvoid  value = reinterpret_cast<Pvoid>(&v);
 if(data)
 {
  uint32_t val_sz = otd_data_calc_size(data->otd_type,1);
  if(val_sz>sizeof(v))
  {
   Pvoid __value = malloc(val_sz);
   if(__value)
      {
        memset(__value,0,val_sz);
        value = __value;
      }
      else
       val_sz = sizeof (v);
  }

 for(uint8_t i = data->numbers.loN;i<=data->numbers.hiN;i++)
     otd_set_value(data,i,value,val_sz);

 if(value && value!=reinterpret_cast<Pvoid>(&v))
     free(value);
 }
}

KERTL_API int   otd_fill      (lpotd_data array ,Pvoid val,uint32_t val_sz)
{
  if(array && val && val_sz)
  {
   uint8_t count = otd_numbers_count(&array->numbers);
   for(uint8_t i = 0;i<count;i++)
      otd_set_value(array,i+array->numbers.loN,val,val_sz);
   return count;
  }
 return  0;
}

