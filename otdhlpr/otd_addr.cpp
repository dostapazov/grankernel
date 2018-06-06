#include <otdhlpr/otd_addr.h>


KERTL_API int otd_addr_cmp_part(const lpotd_addr a1,const lpotd_addr a2,int32_t tp_lim)
{
 /*Частичное сравнение адресов*/
 int  cmp_res = 0;
 if(tp_lim >= otd_addr::type_broadcast)
    tp_lim = std::min(a1->get_type(),a2->get_type());
 if(tp_lim<otd_addr::type_broadcast)
 {
  uint8_t * b1 = reinterpret_cast<uint8_t *>(&a1->addr);
  uint8_t * b2 = reinterpret_cast<uint8_t *>(&a2->addr);
  for(int32_t i = 0;i<=tp_lim && !cmp_res;i++,b1++,b2++)
   {
    int cmp_v1 = (int)((uint32_t)(*b1)),cmp_v2 = (int)((uint32_t)(*b2));
    cmp_res = cmp_v1-cmp_v2;
   }
 }
  return cmp_res;
}

KERTL_API bool otd_addr_include(lpotd_addr checked,lpotd_addr mask)
{
 /*
  Проверка включает ли один адрес другой
 */
 bool ret = true;
 otd_addr dwa1(checked->addr),dwa2(mask->addr);
 dwa1.fa&=OTD_FA_ALL;
 dwa2.fa&=OTD_FA_ALL;

 uint8_t* b1 = (uint8_t*)&dwa1.addr;
 uint8_t* b2 = (uint8_t*)&dwa2.addr;
 for(int i = 0;ret && i<otd_addr::addr_type::type_broadcast;i++,b1++,b2++)
 {
  if(i == otd_addr::addr_type::type_fa)
  {
  if((*b1 == *b2 )  /*|| *b1 == OTD_FA_ALL */|| *b2 == OTD_FA_ALL)
      continue;
  }
  else
  if( (*b1 == *b2 )  /*|| *b1 == OTD_ADDR_MAXVALUE*/ || *b2 == OTD_ADDR_MAXVALUE)
      continue;
  ret = false;
 }
 return ret;
}



