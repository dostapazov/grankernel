#if !defined (OTD_ADDR_H)
#define OTD_ADDR_H

#include <kertl.h>

/*Адресация*/
#define OTD_ADDR_ALLMOD 0xFF000000
#define OTD_ADDR_ALLFA  0x00FF0000
#define OTD_ADDR_ALLCP  0x0000FF00
#define OTD_ADDR_ALLPU  0x000000FF
#define OTD_ADDR_ALL    (OTD_ADDR_ALLMOD|OTD_ADDR_ALLFA|OTD_ADDR_ALLCP|OTD_ADDR_ALLPU)
#define OTD_ADDR_MAXVALUE    0xFF

/*Типы ФА для ОТД*/

#define OTD_FA_CALL          0x80 /*Вызов*/
#define OTD_FA_ORDER         0x40 /*Подписка*/
#define OTD_FA_QUERY         (OTD_FA_ORDER|OTD_FA_CALL)
#define OTD_FA_RECEIPT       0x20 /*Квитанция*/
#define OTD_FA_CTRL          0x10 /*Управление/Регулирование*/
#define OTD_FA_SETUP         0x08 /*Различные настройки оборудования*/

#define OTD_FA_DISCRETE      0x00 /* Дискретный параметр       */
#define OTD_FA_ANALOG        0x01 /* Аналоговый параметр       */
#define OTD_FA_ANALOG_INTEGR 0x02 /* Аналоговый параметр1      */
#define OTD_FA_INFORMATION   0x04 /* Запрос информации         */

#define OTD_FA_DICRETE_CTRL  (OTD_FA_CTRL|OTD_FA_DISCRETE) /* Управление дискретными/регулирование через ТУ    */
#define OTD_FA_ANALOG_ADJ    (OTD_FA_CTRL|OTD_FA_ANALOG)   /* Регулирование */

#define OTD_FA_ALL           0x07
#define OTD_FA_DATA_MASK     0x07
#define OTD_FA_SVC_MASK      0xF8


#pragma pack(push,1)

struct  otd_addr
{
  union{
  struct{ uint8_t pu,cp,fa,sb;};
  uint32_t  addr;
  };
  enum addr_type {  type_pu   =   0,type_cp   =   1,type_fa   =   2,type_sb   =   3,type_broadcast = 4,type_undef = -1};
  otd_addr()
          :addr(OTD_ADDR_ALL){}

  otd_addr(const uint32_t _addr)
          :addr(_addr){}

  otd_addr(uint8_t _pu, uint8_t _cp, uint8_t _fa, uint8_t _sb)
          :pu(_pu),cp(_cp),fa(_fa),sb(_sb){}

  otd_addr(const otd_addr & src){*this = src;}
  otd_addr & operator  = (const otd_addr & src){addr = src.addr;return *this;}
  bool      is_pu() {return (pu!=OTD_ADDR_MAXVALUE && cp == OTD_ADDR_MAXVALUE) ? true : false; }
  bool      is_cp() {return (pu!=OTD_ADDR_MAXVALUE && cp != OTD_ADDR_MAXVALUE && (fa == OTD_ADDR_MAXVALUE || fa == OTD_FA_ALL)) ? true : false;}
  bool      is_fa() {return (pu!=OTD_ADDR_MAXVALUE && cp != OTD_ADDR_MAXVALUE && fa != OTD_ADDR_MAXVALUE  && sb == OTD_FA_ALL ) ? true : false;}
  bool      is_sb() {return (pu!=OTD_ADDR_MAXVALUE && cp != OTD_ADDR_MAXVALUE && fa != OTD_ADDR_MAXVALUE  && sb != OTD_FA_ALL ) ? true : false;}
  addr_type get_type();
};

typedef  otd_addr* lpotd_addr;
#pragma pack(pop)

inline otd_addr::addr_type otd_addr::get_type()
{
  if(is_pu()) return type_pu;
  if(is_cp()) return type_cp;
  if(is_fa()) return type_fa;
  if(is_sb()) return type_sb;
  return this->addr == OTD_ADDR_ALL ? type_broadcast :  type_undef;
}


 #ifdef __cplusplus
inline bool otd_fa_isquery(uint8_t fa){return ((fa!= OTD_ADDR_MAXVALUE) && (fa&OTD_FA_QUERY)) ? true : false;}
inline bool otd_fa_isall  (uint8_t fa){return ((fa&OTD_FA_ALL)==OTD_FA_ALL) ? true : false;}
inline bool otd_fa_isdata (uint8_t fa){return (fa&~OTD_FA_DATA_MASK) ? false : true;}
inline bool otd_fa_isctrl (uint8_t fa)
       {
        if(!otd_fa_isall(fa))
         {
           fa&=OTD_FA_SVC_MASK;
           return (fa == OTD_FA_DICRETE_CTRL || fa == OTD_FA_ANALOG_ADJ ) ? true :false;
         }
        return false;
       }
inline bool otd_fa_issetup(uint8_t fa)
       {return ((fa!= OTD_ADDR_MAXVALUE) && (fa&OTD_FA_SETUP) && 0==(fa&~(OTD_FA_ALL|OTD_FA_SETUP))) ? true : false;}

extern "C"
{
  KERTL_API bool otd_addr_include(lpotd_addr checked,lpotd_addr mask);
  KERTL_API int  otd_addr_cmp_part(const lpotd_addr a1,const lpotd_addr a2,int32_t tp_lim);
}

 #endif // __cplusplus
#endif // OTD_ADDR_H
