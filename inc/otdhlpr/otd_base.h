#if !defined OTD_BASE_H
#define OTD_BASE_H

#include <kertl.h>


#pragma pack(push,1)

struct otd_numbers
{
  uint8_t loN;
  uint8_t hiN;
};

typedef otd_numbers * lpotd_numbers;


#define OTD_TUOP_NOP        0
#define OTD_TUOP_ON         0x0001             //Включить
#define OTD_TUOP_OFF        0x0002             //Отключить
#define OTD_TROP_MORE       OTD_TUOP_ON      //Больше
#define OTD_TROP_LESS       OTD_TUOP_OFF     //Меньше
#define OTD_TROP_SET        0x0004             //Установка параметра
#define OTD_TUTROP_CANCEL   0x0008             //Отмена
#define OTD_TUTROP_UNDO     OTD_TUTROP_CANCEL

#define OTD_TUTR_OPMASK      0x000F

#define OTD_TUTR_CMDSELECT   0x0010  //Выбор объекта для ТУ
#define OTD_TUTR_CMDENABLE   0x0020  //Разрешение ТУ/ТР
#define OTD_TUTR_CMDDESELECT 0x0040  //Отмена выбора объекта
#define OTD_TUTR_CMDMASK     0x00F0

#define OTD_TUTR_CMDATTR_CHECK_SUCCESS 0x0001
#define OTD_TUTR_CMDATTR_INVERSE       0x0002

struct otd_tutr
{
 union{
  struct
  {
    uint16_t  cmd;
    uint16_t  cmd_attr;
  };
  uint32_t command;
 };
 uint64_t                param;
 otd_tutr():command(0),param(0){}

};

typedef  otd_tutr* lpotd_tutr;



#define OTD_TYPE_MASK           0x7000 //Маска базовых типов
#define OTD_BIT_TYPE            0x1000
#define OTD_BYTE_TYPE           0x2000
#define OTD_FLOAT_TYPE          0x4000
#define OTD_SIGNED_FLOAT_TYPE   0xC000 //Float type always signed
#define OTD_SIGNED_TYPE         0x8000

#define OTD_GET_TYPE(type)   (type&OTD_TYPE_MASK)
#define OTD_TYPE_SIGNED(TYPE)(type&OTD_SIGNED_TYPE)


#define OTD_TYPE_LEN  0x00FF //Маска длины типа

#define OTD_DISCRETE        (OTD_BIT_TYPE |1)
#define OTD_BYTE            (OTD_BYTE_TYPE|sizeof(uint8_t))
#define OTD_ANALOG_BYTE     (OTD_BYTE_TYPE|sizeof(uint8_t))
#define OTD_ANALOG_WORD     (OTD_BYTE_TYPE|sizeof(int16_t))
#define OTD_ANALOG_SHORT    (OTD_BYTE_TYPE|OTD_SIGNED_TYPE|sizeof(int16_t))
#define OTD_ANALOG_DWORD    (OTD_BYTE_TYPE|sizeof(int32_t))
#define OTD_ANALOG_LONG     (OTD_BYTE_TYPE|OTD_SIGNED_TYPE|sizeof(int32_t))
#define OTD_ANALOG_QWORD    (OTD_BYTE_TYPE|sizeof(int64_t))
#define OTD_ANALOG_LONGLONG (OTD_BYTE_TYPE|OTD_SIGNED_TYPE|sizeof(int64_t))
#define OTD_FLOAT           (OTD_FLOAT_TYPE|sizeof(float))
#define OTD_SIGNED_FLOAT    (OTD_SIGNED_FLOAT_TYPE|sizeof(float))
#define OTD_DOUBLE          (OTD_FLOAT_TYPE|sizeof(double))
#define OTD_SIGNED_DOUBLE   (OTD_SIGNED_FLOAT_TYPE|sizeof(double))
#define OTD_TUTR_COMMAND    (OTD_BYTE_TYPE|sizeof(uint32_t))
#define OTD_TR_COMMAND_SET  (OTD_BYTE_TYPE|(sizeof(uint32_t)+sizeof(uint64_t)))

/*Массив ОТД данных*/
struct otd_data
{
 uint16_t    otd_type;
 otd_numbers numbers;
 uint8_t     data[KERTL_ANYZISE];
};

typedef otd_data* lpotd_data;

#pragma pack(pop)




#ifdef __cplusplus
extern "C"{
#endif

KERTL_API int        otd_get_value(lpotd_data array,uint8_t number,Pvoid value,size_t sz);
KERTL_API int        otd_set_value(lpotd_data array,uint8_t number,Pvoid value,size_t sz);
KERTL_API int        otd_inc_value(lpotd_data array,uint8_t number,bool no_zero);
KERTL_API int        otd_dec_value(lpotd_data array,uint8_t number,bool no_zero);
KERTL_API unsigned   otd_copy     (lpotd_data dest,lpotd_data src,uint8_t start,uint8_t count);
KERTL_API void       otd_clear    (lpotd_data data);
KERTL_API int        otd_fill      (lpotd_data array ,Pvoid val,uint32_t val_sz);

#ifdef __cplusplus
//extern "C"{
}

inline uint8_t otd_numbers_count   (lpotd_numbers n){ return n ? 1 + (n->hiN - std::max(n->loN,n->hiN)) : 0;}

inline void     otd_numbers_set     (lpotd_numbers n
                                    ,const uint8_t start
                                    ,const uint8_t count)
{n->loN = start; n->hiN = count  ? start+count-1 : start; }

inline bool     otd_number_check   (lpotd_numbers n
                                    ,const uint8_t num)
{return (n && n->loN<=num && n->hiN >= num) ? true : false;}


inline unsigned __fastcall otd_data_calc_size(const uint16_t type, const uint8_t count)
{
 unsigned ret;
 if(OTD_GET_TYPE(type)==OTD_BIT_TYPE)
   ret = count*(type&OTD_TYPE_LEN)/8 + ((count*(type&OTD_TYPE_LEN)%8)? 1:0);
   else
   ret = count*(type&OTD_TYPE_LEN);
   return ret;
}

inline unsigned __fastcall otd_data_get_data_size(lpotd_data od)
{
 return otd_data_calc_size(od->otd_type,otd_numbers_count(&od->numbers));
}


inline unsigned __fastcall otd_data_get_size(lpotd_data od)
{
 unsigned ret = sizeof(*od)-sizeof (od->data[0]);
          ret+=otd_data_get_data_size(od);
 return ret;
}

inline unsigned  otd_zero_value(lpotd_data array,uint8_t number)
{
 uint64_t v = 0;
 return otd_set_value(array,number,&v,sizeof(v));
}

#endif


#endif
