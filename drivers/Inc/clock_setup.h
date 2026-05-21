/*
 * clock_setup.h
 *
 *  Created on: Mar 30, 2026
 *      Author: Rishikesh
 */

#ifndef INC_CLOCK_SETUP_H_
#define INC_CLOCK_SETUP_H_

#define __vo volatile
#define SYST_CSR_BASEADDR ((Systick_regdef_t*) 0xE000E010)



typedef struct
{
 __vo uint32_t syst_csr ;
 __vo uint32_t syst_rvr ;
 __vo uint32_t syst_cvr ;
 __vo uint32_t syst_calib ;


}Systick_regdef_t;



void Systick_configure(uint32_t reload_val);
void clock_set_HSE(void);

#endif /* INC_CLOCK_SETUP_H_ */
